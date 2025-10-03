import java.util.List;

public class Predmet implements IInformacije{
    private String naziv;
    private String odgovorniProfesor;
    private List<String> asistenti;

    public Predmet(String naziv, String odgovorniProfesor, List<String> asistenti, Double ECTS) {
        this.naziv = naziv;
        this.odgovorniProfesor = odgovorniProfesor;
        this.asistenti = asistenti;
        this.ECTS = ECTS;
    }

    private Double ECTS;
    public String DajInformacije()
    {
        return "Predmet: " + naziv + ", odgovorni profesor: " + odgovorniProfesor;
    }
}
