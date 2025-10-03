import java.io.FileReader;
import java.io.IOException;
import java.util.*;

import controller.PredmetController;
import model.Osoba;
import model.Predmet;
import model.Uloga;
import view.PredmetView;

public class Main {
    public static void main(String[] args) {

        Predmet model = new Predmet("Placeholder", 5.0);
        PredmetView view = new PredmetView();
        PredmetController controller = new PredmetController(model, view);

        controller.ucitajPredmetIzFajla("predmet.txt");

        System.out.println("Naziv predmeta: " + model.getNaziv());
        System.out.println("ECTS vrijednost: " + model.getECTS());
    }


}