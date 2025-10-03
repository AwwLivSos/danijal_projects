package controller;

import model.Predmet;
import view.PredmetView;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class PredmetController {
    private Predmet model;
    private PredmetView view;

    public PredmetController(Predmet model, PredmetView view) {
        this.model = model;
        this.view = view;
    }

    public void azurirajNaziv() {
        try {
            model.setNaziv(view.getUlazniTekst());
            view.setPoruka("Naziv predmeta uspješno ažuriran!");
        } catch (Exception e) {
            view.setPoruka("Greška: " + e.getMessage());
        }
    }

    public void azurirajECTS() {
        try {
            model.setECTS(Double.parseDouble(view.getUlazniTekst()));
            view.setPoruka("ECTS vrijednost uspješno ažurirana!");
        } catch (Exception e) {
            view.setPoruka("Greška: " + e.getMessage());
        }
    }

    public void ucitajPredmetIzFajla(String filePath) {
        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String naziv = br.readLine();
            Double ECTS = Double.parseDouble(br.readLine());

            model.setNaziv(naziv);
            model.setECTS(ECTS);

            view.setPoruka("Predmet uspješno učitan iz fajla: " + naziv + ", ECTS: " + ECTS);
        } catch (IOException | IllegalArgumentException e) {
            view.setPoruka("Greška prilikom učitavanja predmeta: " + e.getMessage());
        }
    }

}
