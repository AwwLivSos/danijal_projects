package etf.ri.rma.newsfeedapp.model

import etf.ri.rma.newsfeedapp.model.Tag

data class NewsItem(
    val uuid: String,
    val title: String,
    val snippet: String,
    val imageUrl: String?,
    val category: String,
    val isFeatured: Boolean,
    val source: String,
    val publishedDate: String,
    val imageTags: ArrayList<Tag> = ArrayList()
) 