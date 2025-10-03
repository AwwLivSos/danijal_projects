import java.util.ArrayList;

public class Main{
    public static void main(String[] args){
        ArrayList<String> imena = PodaciDAO.dajImena();
        System.out.println(imena.size());
        for(String ime : imena){
            System.out.println(ime);
        }
    }
}