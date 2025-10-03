package controller;

import model.Osoba;
import model.OsobaModel;
import view.OsobaView;

public class OsobaController {
    private OsobaModel model;
    private OsobaView view;

    public OsobaController(OsobaModel model, OsobaView view)
    {
        this.model = model;
        this.view = view;
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
