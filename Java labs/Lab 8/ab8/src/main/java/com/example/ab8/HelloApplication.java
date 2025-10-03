package com.example.ab8;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;
import model.OsobaModel;
import controller.OsobaController;
import view.OsobaView;

import java.io.IOException;
                                //nasljeđena iz klase Application, sto je valjda kao da nasljeđuje klasu Object u javi, kao osnova sevga
public class HelloApplication extends Application {
    @Override       //prima argument stage, sto je prozor koji gleda korisnik, a baca IOExepction, lkoji je povezan sa fxmlom
    public void start(Stage stage) throws IOException {  //metoda start je glavna metoda

        OsobaModel osobaModel = new OsobaModel();
        OsobaView osobaView = new OsobaView();

        FXMLLoader fxmlLoader = new FXMLLoader(HelloApplication.class.getResource("hello-view.fxml"));  //ovo ucitava datoteku
        fxmlLoader.setController(new OsobaController(osobaModel));

        Scene scene = new Scene(fxmlLoader.load(), 300, 700);      //"postavljamo scenu", sa fxml datotekom i ovim svim argumentima (sirina, visina, gdje sta ide itd)

        stage.setTitle("Dodaj osobu");
        stage.setScene(scene);
        stage.show();                   //ova metoda zapravo cini pozornicu vidljivom
    }

    public static void main(String[] args) {
        launch();                               //metoda main lansira cjelokupni program, poziva start
    }
}