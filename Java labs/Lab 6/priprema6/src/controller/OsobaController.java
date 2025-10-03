package controller;

import model.Osoba;
import view.OsobaView;

public class OsobaController {
    private Osoba model;
    private OsobaView view;

    public OsobaController(Osoba model, OsobaView view) {
        this.model = model;
        this.view = view;
    }

    public void azurirajIme() {
        try {
            model.setIme(view.getUlazniTekst());
            view.setPoruka("Ime je uspješno ažurirano!");
        } catch (Exception e) {
            view.setPoruka("Greška: " + e.getMessage());
        }
    }
}
