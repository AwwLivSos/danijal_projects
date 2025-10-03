import java.util.*;

public abstract class Namirnice implements Zdravlje{
    protected String porijeklo;
    protected List<Integer> podaci;

    public Namirnice(String porijeklo, List<Integer> podaci) {
        this.porijeklo = porijeklo;
        this.podaci = podaci;
    }
    
    public String getPorijeklo() {
        return porijeklo;
    }

    public void setPorijeklo(String porijeklo) {
        this.porijeklo = porijeklo;
    }

    public List<Integer> getPodaci() {
        return podaci;
    }

    public void setPodaci(List<Integer> podaci) {
        this.podaci = podaci;
    }

    public double DajBrojKalorija(){
        double suma=0;
        for(var a : podaci){
            suma+=a;
        }
        return suma;
    }
}

