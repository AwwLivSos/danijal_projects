import java.util.List;

public class Povrce extends Namirnice{
    private String latinski;
    private String zemlja;

    public Povrce(String porijeklo, List<Integer> podaci, String lat, String zem) {
        super(porijeklo, podaci);
        latinski = lat;
        zemlja = zem;
    }

    @Override
    public boolean zdravlje(double koeficijent) {
        return DajBrojKalorija()<100 && (koeficijent<0.7 && koeficijent>0.5);
    }
}

