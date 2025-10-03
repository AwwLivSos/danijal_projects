#Drive mount
from google.colab import drive
drive.mount('/content/drive')
import glob
image_paths = glob.glob('/content/drive/MyDrive/DPS MRI slike/meningioma/*.jpg')

#Main functions and imports
import cv2
import numpy as np
import matplotlib.pyplot as plt
from skimage import filters, segmentation, color, morphology, exposure
from sklearn.cluster import KMeans
from scipy import ndimage as ndi
from PIL import Image
import os

def show_images(images, titles=None, cols=3, figsize=(15, 10)):
    rows = (len(images) + cols - 1) // cols
    plt.figure(figsize=figsize)
    for i, image in enumerate(images):
        plt.subplot(rows, cols, i + 1)
        plt.imshow(image, cmap='gray')
        if titles: plt.title(titles[i])
        plt.axis('off')
    plt.tight_layout()
    plt.show()

def load_image(path, target_size=(256, 256)):
    img = cv2.imread(path)
    img = cv2.resize(img, target_size)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    return img, gray

def preprocess_image(gray_img):
    equalized = cv2.equalizeHist(gray_img)
    return equalized

#Test
img, gray = load_image(image_paths[1])
equalized = preprocess_image(gray)

show_images([gray, equalized], ["Original (Grayscale)", "Histogram Equalized"])

#Antisotropic method
def segment_anisotropic(img_gray):
    smooth = cv2.edgePreservingFilter(img_gray, flags=1, sigma_s=60, sigma_r=0.4)

    norm_img = cv2.normalize(smooth, None, 0, 255, cv2.NORM_MINMAX)
    _, thresh = cv2.threshold(norm_img, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)

    kernel = np.ones((3, 3), np.uint8)
    clean = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel, iterations=1)

    return clean

#Otsu segmentation
def segment_otsu(img_gray):
    blurred = cv2.GaussianBlur(img_gray, (5, 5), 0)

    _, thresh = cv2.threshold(blurred, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)

    kernel = np.ones((3, 3), np.uint8)
    clean = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel, iterations=1)

    return clean

#KMeans segmentation
def segment_kmeans(img_gray, k=2):
    Z = img_gray.reshape((-1, 1)).astype(np.float32)

    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
    _, labels, centers = cv2.kmeans(Z, k, None, criteria, 10, cv2.KMEANS_RANDOM_CENTERS)

    segmented = centers[labels.flatten()]
    segmented_img = segmented.reshape(img_gray.shape).astype(np.uint8)

    _, binary = cv2.threshold(segmented_img, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)

    kernel = np.ones((3, 3), np.uint8)
    clean = cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel, iterations=1)

    return clean

#Watershed method
def segment_watershed(img_gray):
    _, binary = cv2.threshold(img_gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)

    kernel = np.ones((3, 3), np.uint8)
    opening = cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel, iterations=2)

    sure_bg = cv2.dilate(opening, kernel, iterations=3)

    dist_transform = cv2.distanceTransform(opening, cv2.DIST_L2, 5)
    _, sure_fg = cv2.threshold(dist_transform, 0.7 * dist_transform.max(), 255, 0)

    sure_fg = np.uint8(sure_fg)
    unknown = cv2.subtract(sure_bg, sure_fg)

    _, markers = cv2.connectedComponents(sure_fg)

    markers = markers + 1

    markers[unknown == 255] = 0

    img_color = cv2.cvtColor(img_gray, cv2.COLOR_GRAY2BGR)
    markers = cv2.watershed(img_color, markers)

    segmentation_result = np.zeros_like(img_gray)
    segmentation_result[markers == -1] = 255

    return segmentation_result

#Region growing method
def segment_region_growing(img_gray, seed=None, threshold=10):
    height, width = img_gray.shape

    # Default seed: image center
    if seed is None:
        seed = (height // 2, width // 2)

    visited = np.zeros_like(img_gray, dtype=bool)
    mask = np.zeros_like(img_gray, dtype=np.uint8)

    seed_val = int(img_gray[seed])
    stack = [seed]

    while stack:
        x, y = stack.pop()
        if visited[x, y]:
            continue
        visited[x, y] = True

        current_val = int(img_gray[x, y])
        if abs(current_val - seed_val) < threshold:
            mask[x, y] = 255
            for dx in [-1, 0, 1]:
                for dy in [-1, 0, 1]:
                    nx, ny = x + dx, y + dy
                    if 0 <= nx < height and 0 <= ny < width:
                        if not visited[nx, ny]:
                            stack.append((nx, ny))
    return mask

#Comparison
def compare_all(img_gray):
    equalized = preprocess_image(img_gray)

    results = [
        ("Anisotropic", segment_anisotropic(equalized)),
        ("Otsu", segment_otsu(equalized)),
        ("Adaptive", segment_adaptive(equalized)),
        ("K-means", segment_kmeans(equalized)),
        ("Watershed", segment_watershed(equalized)),
        ("Region Growing", segment_region_growing(equalized, threshold=12))
    ]

    images = [img_gray, equalized] + [res[1] for res in results]
    titles = ["Original", "Equalized"] + [res[0] for res in results]

    show_images(images, titles=titles, cols=4, figsize=(20, 10))

#Image overlay
def extract_and_overlay(original_img, binary_mask):
    num_labels, labels_im = cv2.connectedComponents(binary_mask)
    if num_labels <= 1:
        return original_img

    areas = [np.sum(labels_im == i) for i in range(1, num_labels)]
    largest_label = np.argmax(areas) + 1
    tumor_mask = np.uint8(labels_im == largest_label) * 255

    if len(original_img.shape) == 2:
        original_img = cv2.cvtColor(original_img, cv2.COLOR_GRAY2BGR)

    red_mask = np.zeros_like(original_img)
    red_mask[:, :, 2] = tumor_mask

    overlay = cv2.addWeighted(original_img, 0.8, red_mask, 0.4, 0)

    return overlay

def overlay_edges(original_img, edge_mask):
    if len(original_img.shape) == 2:
        original_img = cv2.cvtColor(original_img, cv2.COLOR_GRAY2BGR)

    red_mask = np.zeros_like(original_img)
    red_mask[:, :, 2] = edge_mask

    overlay = cv2.addWeighted(original_img, 0.9, red_mask, 0.9, 0)
    return overlay