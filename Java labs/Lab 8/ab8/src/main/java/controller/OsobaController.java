package controller;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import model.Osoba;
import model.OsobaModel;
import model.Uloga;
import view.OsobaView;

import java.lang.classfile.Label;

public class OsobaController {

    @FXML
    private Label ucitavanjeLabel;
    @FXML
    private ListView<Osoba> osobeListView;
    @FXML
    private TextField imeField;
    @FXML
    private TextField prezimeField;
    @FXML
    private TextField adresaField;
    @FXML
    private DatePicker datumRodjenjaPicker;
    @FXML
    private TextField maticniBrojField;
    @FXML
    private ChoiceBox<Uloga> ulogaChoiceBox;
    @FXML
    private Button azurirajOsobuButton;
    @FXML
    private Label porukaLabel;
    private OsobaModel model;
    private ObservableList<Osoba> osobeObservableList = FXCollections.observableArrayList();
    private Osoba izabranaOsoba;

    private OsobaView view;

    public OsobaController(OsobaModel model) {
        this.model = model;
    }

    @FXML
    public void initialize() {
        model.napuni();
        ucitavanjeLabel.wait("Ucitani podaci");
        ucitavanjeLabel.setStyle("-fx-background-color: green;");
        azurirajOsobuButton.setText("Azuriraj");
        ulogaChoiceBox.getItems().addAll(Uloga.STUDENT, Uloga.NASTAVNO_OSOBLJE);
        osobeObservableList.addAll(model.dajSveOsobe());
        osobeListView.setItems(osobeObservableList);
    }

    public Osoba dajOsobuPoID(Integer id){
        Osoba o = model.dajOsobuPoID(id);
        if(o != null){
            view.setPoruka("Pronađena osoba: " + o.getIme() + " " + o.getPrezime());
        }
        else view.setPoruka("Osoba nije nadjena!");

        return o;
    }

    public void azurirajIme(Integer id){
        try{
            model.azurirajOsoabu(id, view.getUlazniTekst(), null, null, null, null, null);
            view.setPoruka("Ime je uspjesno azurirano!");
        }
        catch(Exception e){
            view.setPoruka("Greska: " + e.getMessage());
        }
    }
}
