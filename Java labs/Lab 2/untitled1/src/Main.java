import Izuzeci.PremladStudentException;
import Izuzeci.StudentBuducnostException;
import Klase.Odsjek;
import Klase.Student;

import java.util.ArrayList;
import java.util.Date;
import java.util.Scanner;

public class Main {
    public static void main(String[]args){
        Scanner scanner = new Scanner(System.in);
        boolean uspjesanUnos = false;
        Student student = null;
        while (!uspjesanUnos) {
            try {
                System.out.printf("Datumrođenja(dd/mm/yyyy):");
                String datumString = scanner.nextLine();
                int godina = Integer.valueOf(datumString.substring(6)) - 1900;
                int mjesec = Integer.valueOf(datumString.substring(3, 5)) - 1;
                int dan = Integer.valueOf(datumString.substring(0, 2));
                Date datumDate = new Date(godina, mjesec, dan);
                student = new Student("Ime", "Prezime", datumDate, "12345", Odsjek.RI, 2);
                uspjesanUnos = true;

            } catch (PremladStudentException e) {
                System.out.println(e.getMessage());
                return;
            } catch (StudentBuducnostException e) {
                System.out.println(e.getMessage());
                System.out.println("Molimo ponovite unos datuma rođenja!");
            }
        }

        String unosOcjena = "";
        System.out.println("Unesi ocjene");
        unosOcjena = scanner.nextLine();
        if(!unosOcjena.isEmpty()){
            String[] ocjeneString = unosOcjena.split(",");
            ArrayList<Integer> ocjeneInteger = new ArrayList<>();
            for(var a : ocjeneString){
                ocjeneInteger.add(Integer.valueOf(a));
            }
            student.setOcjene(ocjeneInteger);
        }else{
            student.setOcjene(new ArrayList<Integer>());
        }

        System.out.println("Unos studenta uspješan! " + student);
    }
}