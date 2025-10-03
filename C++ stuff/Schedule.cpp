#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <iomanip>
#include <algorithm>

int timeToMinutes(const std::string& time) {
    int hours = std::stoi(time.substr(0, 2));
    int minutes = std::stoi(time.substr(3, 2));
    return hours * 60 + minutes;
}
//dvije helper funkcije za manipulaciju vremenima, pretavrano string tipa "14:30" u adekvatan broj minuta, pa onda i obratno
std::string minutesToTime(int minutes) {
    int hours = minutes / 60;
    int mins = minutes % 60;
    return (hours < 10 ? "0" : "") + std::to_string(hours) + ":" + (mins < 10 ? "0" : "") + std::to_string(mins);
}

struct Predmet{
    std::string ime;
    std::tuple<std::string, std::string, std::string> vremenaPredavanja;                                    //tuple se sastoji od imena dana, pocetka predavanja i kraja predavnaja
    std::vector<std::pair<int, std::tuple<std::string, std::string, std::string>>> vremenaTutorijala;       //int je broj grupe, a svaka grupa ce imati svoje termine
} ra{"RA", {"Srijeda", "09:00", "11:30"}, {}},                                                              //ideja je da znam samo pocetak i kraj pa da se automatski popuni, nisam siguran da li ce raditi
  afj{"AFJ", {"Cetvrtak", "09:00", "11:30"}, {}},
  ooad{"OOAD", {"Srijeda", "12:00", "14:30"}, {}}, 
  orm{"ORM", {"Petak", "15:00", "17:00"}, {}}, 
  //redovni vs izborni
  rma{"RMA", {"Utorak", "09:00", "11:30"}, {}},
  dps{"DPS", {"Utorak", "12:00", "14:30"}, {}},
  us{"US", {"Cetvrtak", "13:00", "14:30"}, {}},
  cci{"CCI", {"Petak", "09:00", "11:30"}, {}};

class Raspored{
    std::map<std::string, std::map<std::string, std::string>> schedule;
public:
    void dodajUMojRaspored(const std::string& aktivnost, const std::string& dan, const std::string& startTime, const std::string& endTime) {
        int start = timeToMinutes(startTime);
        int end = timeToMinutes(endTime);
        int step = 30;  //intervali od 30 minuta

        if(!jelDostupno(dan, start, end)) throw std::domain_error("Taj termin je vec popunjen (" + schedule[dan][endTime] +"). Molimo odaberite drugi.");

        for (int t = start; t <= end; t += step) {
            std::string timeSlot = minutesToTime(t);
            schedule[dan][timeSlot] = aktivnost;
        }
    }

    bool jelDostupno(const std::string& day, int start, int end) {
        for (int t = start; t <= end; t += 30) {
            if (schedule[day].count(minutesToTime(t))) return false; 
        }
        return true; 
    }

    void prikaziRaspored() {
        const int width = 20;
        const std::string days[] = {"Ponedjeljak", "Utorak", "Srijeda", "Cetvrtak", "Petak"};
        const std::string times[] = {"08:30", "09:00", "10:00", "10:30", "11:00", "11:30", "12:00", "12:30", "13:00", "13:30", "14:00", "14:30", "15:00", "16:00", "16:30", "17:00", "18:00"};

        std::cout << std::setw(10) << "Vrijeme";                                //prvi red
        for (const auto& day : days)
            std::cout << std::setw(width) << day;
        std::cout << "\n";

        std::cout << std::string(10 + width * 5, '-') << "\n";                  //separator linija

        for (const auto& time : times) {                                        //ispisi svaki red sa satnicom sa lijeve strane
            std::cout << std::setw(10) << time;                                 //satnica
            for (const auto& day : days) {
                if (schedule[day].count(time))
                    std::cout << std::setw(width) << schedule[day][time];       //aktivnost
                else std::cout << std::setw(width) << "-";                      //slobodno
            }
            std::cout << "\n";
        }
    }
};

void popuniSveTutorijale(){
    afj.vremenaTutorijala = {{1, {"Ponedjeljak", "12:00", "13:30"}}, {2, {"Ponedjeljak", "14:00", "15:00"}}};
    ooad.vremenaTutorijala = {{1, {"Utorak", "09:00", "10:00"}}, {2, {"Utorak", "10:30", "11:30"}},
                              {3, {"Srijeda", "15:00", "16:00"}}, {4, {"Srijeda", "16:30", "17:00"}},
                              {5, {"Cetvrtak", "12:00", "13:00"}}, {6, {"Cetvrtak", "13:30", "14:30"}}};
    orm.vremenaTutorijala = {{1, {"Ponedjeljak", "11:00", "12:00"}}, {2, {"Ponedjeljak", "12:30", "13:30"}},
                             {3, {"Cetvrtak", "13:30", "14:30"}}, {4, {"Petak", "10:00", "11:00"}},
                             {5, {"Cetvrtak", "12:00", "13:00"}}};
    rma.vremenaTutorijala = {{1, {"Utorak", "13:00", "14:30"}}, {2, {"Cetvrtak", "13:30", "14:30"}}, {3, {"Utorak", "12:00", "13:00"}}};
    cci.vremenaTutorijala = {{1, {"Utorak", "10:00", "11:00"}}, {2, {"Utorak", "13:00", "14:00"}},
                             {3, {"Cetvrtak", "12:00", "13:00"}}, {4, {"Cetvrtak", "13:30", "14:30"}}};
    //ra i us dodati kada dođu, dps ima samo jedan termin pa nista od toga
}

int main() {
    while(1){
        std::string i1, i2;
        std::cout << "Odaberite dva izborne predmete (rma, dps, us, cci): ";
        std::cin >> i1 >> i2;
        popuniSveTutorijale();

        std::map<std::string, Predmet> izborniPredmeti = {{"rma", rma}, {"dps", dps}, {"us", us}, {"cci", cci}};
        while(1){
            if (izborniPredmeti.count(i1) == 0 || izborniPredmeti.count(i2) == 0 || i1 == i2){
                std::cout << "Neispravan unos! Morate odabrati dva razlicita dostupna izborna predmeta.\n";
                std::cin>>i1>>i2;
            }
            else break;
        }
        auto izborni1=izborniPredmeti[i1], izborni2=izborniPredmeti[i2];

        Raspored r;
        for (const auto& p : {ra, afj, ooad, orm, izborni1, izborni2}) {
            auto session = p.vremenaPredavanja;
            r.dodajUMojRaspored(p.ime+"-P", std::get<0>(session), std::get<1>(session), std::get<2>(session));
        }

        if(i1=="dps" || i2 =="dps") r.dodajUMojRaspored("DPS-T", "Utorak", "15:00", "15:00");
        std::cout<<"Racunajuci da cete ici na sve, vas obavezni raspored za sada izgleda ovako: \n";
        r.prikaziRaspored();

        std::cout<<"Sada, odaberite grupe za tutorijale/labove.\n";
        
        std::vector<Predmet> sviPredmeti = {izborni1, izborni2, afj, ra, ooad, orm};

        for (const auto& subject: sviPredmeti) {
            if (subject.ime == "DPS") {
                std::cout<<"Za DPS postoji samo jedna grupa za tutorijal, koja je automatski dodana.\n";
                continue;
            }
            
            std::vector<int> validGroups;
            std::cout << "Dostupni tutorijali za " << subject.ime << ":\n";
            
            for (const auto& grupe : subject.vremenaTutorijala) {
                if (r.jelDostupno(std::get<0>(grupe.second), timeToMinutes(std::get<1>(grupe.second)), timeToMinutes(std::get<2>(grupe.second)))) {
                    std::cout << "Grupa " << grupe.first << " (" << std::get<0>(grupe.second) << ", " << std::get<1>(grupe.second) << " - " << std::get<2>(grupe.second) << ")\n";
                    validGroups.push_back(grupe.first);
                }
            }
            
            if (validGroups.empty()) {
                std::cout << "Nazalost, nema dostupnih grupa za " << subject.ime << " zbog konflikta.\n";
                continue;
            }

            int chosenGroup;
            while (true) {
                std::cout << "Odaberite broj grupe: ";
                std::cin >> chosenGroup;
                if (std::find(validGroups.begin(), validGroups.end(), chosenGroup) != validGroups.end()) break;
                if(!std::cin){
                    std::cin.clear(); 
                    std::cin.ignore(99999, '\n');
                }
                std::cout << "Neispravan unos, pokusajte ponovo.\n";
            }

            auto selectedTime = subject.vremenaTutorijala[chosenGroup-1];
            r.dodajUMojRaspored(subject.ime + "-T", std::get<0>(selectedTime.second), std::get<1>(selectedTime.second), std::get<2>(selectedTime.second));

            r.prikaziRaspored();
        }

        std::cout<<"Vas konacan raspored je prikazan iznad."<<std::endl;

        std::cout << "Zelite li pokrenuti program ponovo? (da/ne): ";
        std::string choice;
        std::cin >> choice;
        if (choice != "da") break; 
    }
    
    return 0;
}