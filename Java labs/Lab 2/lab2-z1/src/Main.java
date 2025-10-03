import Izuzeci.PremladStudentException;
import Izuzeci.StudentBuducnostException;
import Klase.Odsjek;
import Klase.Student;           //unos i importovanje mojih klasa/izuzetaka

import java.util.ArrayList;
import java.util.Date;
import java.util.Scanner;       //unos i importovanje javinih klasa i objekata

public class Main {
    public static void main(String[]args){      //ovo je argument za main metodu

        Scanner scanner = new Scanner(System.in);           //scanner je tipa Scanner koji se koristi za unos na ovakav nacin
        boolean uspjesanUnos = false;
        Student student = null;                             //null "pokazivac" ili referenca

        while (!uspjesanUnos) {
            try {
                System.out.print("Datumrođenja(dd/mm/yyyy):");              //sintaksa za printanje
                String datumString = scanner.nextLine();                    //scanner.nextLine() znaci da cita jednu liniju, i unosi je u String varijablu datumString
                int godina = Integer.valueOf(datumString.substring(6)) - 1900;
                int mjesec = Integer.valueOf(datumString.substring(3, 5)) - 1;
                int dan = Integer.valueOf(datumString.substring(0, 2));
                //za ovo se koriste fazoni da uzmemo substring od datumString (pod uslovom da je pravilno unesen, sto ne provjeravam) i zatim koristimo sintaksu Integer.valueof ili Integer.parseInt da izvucemo cjelobrojnu vrijednost koja nam treba za dan, mjesec ili godinu

                Date datumDate = new Date(godina, mjesec, dan);             //zatim od poznatih podatak formiramo datum (objekat tipa Date, koji postoji u javi)
                student = new Student("Ime", "Prezime", datumDate, "12345", Odsjek.RI, 2);
                //nakon toga sve potrebne informacije unosimo u novi objekat tipa student, uz odgovarajuci konsturktor
                uspjesanUnos = true;

            } catch (PremladStudentException e) {
                System.out.println(e.getMessage());
                return;
            } catch (StudentBuducnostException e) {
                System.out.println(e.getMessage());
                System.out.println("Molimo ponovite unos datuma rođenja!");
            }   //standardno try-catch grananje
        }

        String unosOcjena = "";
        System.out.println("Unesi ocjene");
        unosOcjena = scanner.nextLine();
        if(!unosOcjena.isEmpty()){
            String[] ocjeneString = unosOcjena.split(",");          //sintaksa za unos niza stringova koji su međusobno razdvojeni zarezom
            ArrayList<Integer> ocjeneInteger = new ArrayList<>();         //nova lista intova koja zapravo mora biti nova lista Integera, sto nije isto
            for(var a : ocjeneString){
                //sintaksa ista kao for-each petlja u c++, samo ovdje nije auto varijabla, nego var
                ocjeneInteger.add(Integer.valueOf(a));
                //dodajemo u listu intove od stringa koji se dobiju ovom sintaksom
                //ovdje bi se mogla provjeriti ispravnost unosa
            }
            student.setOcjene(ocjeneInteger);           //setter za klasu student
        }else{
            student.setOcjene(new ArrayList<Integer>());        //setta da su ocjene prazan niz
        }

        System.out.println("Unos studenta uspješan!\n" + student.to_string());
    }
}

/*
DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd/MM/yyyy");
LocalDate datumDate = LocalDate.parse(datumString, formatter);
bolja sintaksa za datume, novija modernija verzija
 */