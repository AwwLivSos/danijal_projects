import  java.util.*;

public class Main {
    public static void main(String[] args) {
        ArrayList<Integer> nutricioni_podaci = new ArrayList<>();
        nutricioni_podaci.add(1);
        nutricioni_podaci.add(15);
        nutricioni_podaci.add(30);

        Voce jabuka = new Voce("Bangladesh", nutricioni_podaci, "Malus", "BIH");
        Povrce paradajz = new Povrce("Demokratska Republika konmgo", nutricioni_podaci, "Solanum lycopersicum", "Hrvatska");
        Meso burger = new Meso("Mesnica", nutricioni_podaci, VrsteMesa.piletina);
        Prodavac ikea = new Prodavac("Danijal", "Alibegovic", 50, "289A5F4-01");

        System.out.println("Broj kalorija jabuke je " + jabuka.DajBrojKalorija());
        System.out.println("Da li je jabuka zdrava? " + jabuka.zdravlje(0.8));

        System.out.println("Broj kalorija paradajza je " + paradajz.DajBrojKalorija());
        System.out.println("Da li je paradajz zdrav? " + paradajz.zdravlje(0.6));

        System.out.println("Broj kalorija burgera je " + burger.DajBrojKalorija());
        System.out.println("Da li je burger zdrav? " + burger.zdravlje(0.96));

        System.out.println("Da li je prodavac zdrav? " + ikea.zdravlje(100));

    }
}

