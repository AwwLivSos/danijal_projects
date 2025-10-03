import java.util.*;

public class Main {
    public static void main(String[] args) {
        Student s = new Student("Student", "1", "", new Date(98, 2, 2), "12345", 2, 0.0);
        Predmet pr = new Predmet("RPR", "Ajdin Dželo", null, 5.0);
        List<IInformacije> objekti = new ArrayList<IInformacije>();

        objekti.add(s);
        objekti.add(pr);
        for (IInformacije o : objekti)
            System.out.println(o.DajInformacije());
    }
}