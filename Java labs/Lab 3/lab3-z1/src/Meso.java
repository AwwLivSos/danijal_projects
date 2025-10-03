import java.util.List;

public class Meso extends Namirnice{
    private VrsteMesa vrsta;

    public Meso(String porijeklo, List<Integer> podaci, VrsteMesa v) {
        super(porijeklo, podaci);
        vrsta = v;
    }

    @Override
    public double DajBrojKalorija(){
        return super.DajBrojKalorija()*1.2;     //ludost
    }

    @Override
    public boolean zdravlje(double koeficijent) {
        return koeficijent>0.95;
    }
}

