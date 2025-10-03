package controller;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import Model.Osoba;
import Model.OsobaModel;
import Model.Uloga;
import view.OsobaView;

import java.awt.event.ActionEvent;
import java.beans.EventHandler;
import java.lang.classfile.Label;
import java.util.List;

import static Model.OsobaModel.*;

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
    public void initialize() throws Exception {
        OsobaModel.kreirajTabeluAkoNePostoji();
        OsobaModel.isprazniTabeluOsoba();
        OsobaModel.napuniInicijalnimPodacima();
        ucitavanjeLabel.setText("Ucitani podaci");
        ucitavanjeLabel.setStyle("-fx-background-color: green;");
        azurirajOsobuButton.setText("Azuriraj");
        ulogaChoiceBox.getItems().addAll(Uloga.STUDENT, Uloga.NASTAVNO_OSOBLJE);
        ucitajOsobeIzBaze();
        osobeListView.setItems(osobeObservableList);
        // dodavanje listener-a za klik dugmeta
        azurirajOsobuButton.setOnAction(new EventHandler<ActionEvent>()
        {
            @Override
            public void handle(ActionEvent event) {
                azurirajOsobu();
            }
        });
        // dodavanje listener-a za izbor osobe iz listview
        osobeListView.getSelectionModel().selectedItemProperty().addListener((observable, starVrijednost, novaVrijednost)-> {
            if (novaVrijednost != null) {
                izabranaOsoba = novaVrijednost; // azuriranje varijable koja predstavlja trenutno izabranu osobu
                ispuniPolja(novaVrijednost); // ispunjavanje polja detaljima izabrane osobe
                porukaLabel.setVisible(false); // sakrij labelu koja sadrzi poruku
            }
        });
    }

    public Osoba dajOsobuPoID(Integer id){
        Osoba o = model.dajOsobuPoId(id);
        if(o != null){
            view.setPoruka("Pronađena osoba: " + o.getIme() + " " + o.getPrezime());
        }
        else view.setPoruka("Osoba nije nadjena!");

        return o;
    }

    public void azurirajIme(Integer id){
        try{
            model.azurirajOsobu(id, view.getUlazniTekst(), null, null, null, null, null);
            view.setPoruka("Ime je uspjesno azurirano!");
        }
        catch(Exception e){
            view.setPoruka("Greska: " + e.getMessage());
        }
    }

    private void ucitajOsobeIzBaze() throws Exception {
        List<Osoba> osobe = OsobaModel.dajSveOsobe();
        osobeObservableList.setAll(osobe);
    }
}