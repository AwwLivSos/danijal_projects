import java.util.List;

public class Voce extends Namirnice {
    private String latinski;
    private String zemlja;

    public Voce(String porijeklo, List<Integer> podaci, String lat, String zem) {
        super(porijeklo, podaci);
        latinski = lat;
        zemlja = zem;
    }

    @Override
    public boolean zdravlje(double koeficijent) {
        return DajBrojKalorija()<50 && koeficijent>0.75;
    }
}

