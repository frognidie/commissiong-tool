const unsigned char *strings_dim_slop_titile[] ={"Pente de variation ","Dimmer slope","curva di variazione","Dimmerkurve","pendiente de variaci�n"};
const unsigned char *strings_dim_rise_titile[] ={"Montante","Rising","ascendente","steigend","ascendente"}; 
const unsigned char *strings_dim_fall_titile[] ={"Descendante","Falling","discendente","fallend","descendente"}; 
const unsigned char *strings_dim_sec_titile[] ={"s","s","sec","S","s"}; 

const unsigned char *strings_aux_input_titile[] ={"Entr�e auxiliaire","Auxiliary input","ingresso ausiliario","Zusatzeingang","entrada auxiliar"}; 
const unsigned char *strings_aux_input_mode_titile[] ={"Voie 1","Mode","canale 1","Spur 1","v�a 1"}; 
const unsigned char *strings_aux_input_short_pulse_titile[] ={"Temps appui court ","Short press duration","durata pressione breve","Kurzdruckdauer","tiempo de pulsaci�n corta"}; 
const unsigned char *strings_button_titile[]={" Poussoir"," Button"," pulsante"," Druckknopf"," pulsador"};
const unsigned char *strings_switch_titile[]={"Iterrupteur"," Switch"," inter."," Schalter"," interr."};


//const unsigned char *strings_on_level_titile[] ={"Niveau d�allumage","ON level","livello di illuminazione","Leuchtst�rke","nivel de iluminaci�n"}; 

const unsigned char *strings_hours_titile[] = {"heure","hour","ora","Uhrzeit","hora"};

const unsigned char *strings_extend_mode_state_titile[] ={ \
"Inactif",                  "Inactive",                 "OFF",                      "inactivo",                 "deaktiv.",   
"Actif",                    "Active",                   "ON",                       "activo",                   "aktiv.",                   // 2.4.0, modify Italian translation
                // 2.4.0, modify Italian translation
}; 


const unsigned char *strings_week_day_title[] ={"Jour de la semaine",	"Week day",	"giorno della settimana",	"Wochentag",	"d�a de la semana"};

const unsigned char *strings_operating_time_titile[] ={"Temps d'utilisation","Operating time","durata di","Betriebsdauer","tiempo de"}; 
const unsigned char *strings_operating_time1_titile[] ={"fonctionnement (ON)","(ON)","funzionamento (ON)","(EIN)","funcionamiento (ON)"}; 

const unsigned char *strings_operating_time_hours_titile[] ={"heures","Hour","ore","Stunden","horas"}; 

const unsigned char *strings_date_week_title[]={"Lundi","Mardi","Mercredi","Jeudi","Vendredi","Samedi","Dimanche",\
"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday",\
"lunedi","martedi","mercoledi","giovedi","venerdi","sabato","domenica",\
"Montag","Dienstag","Mittwoch","Donnerstag","Freitag","Samstag","Sonntag",\
"lunes","martes","mi�rcoles","jueves","viernes","s�bado","domingo",};


const unsigned char *strings_scheduled_week[]={" L "," M" ," M"," J "," V "," S "," D ",\
" M "," T "," W "," T "," F "," S "," S ",\
"L","M","M","G","V","S","D",\
"M","D","M","D","F","S","S",\
"L","M","M","J","V","S","D"};

const unsigned char *string_scheduled_start_titile[]={"D�but",	"Start",	"inizio",	"Anfang",	"inicio"};
const unsigned char *string_scheduled_stop_titile[]={"Fin",	"Stop",	"fine",	"Ende",	"final"};


const unsigned char *string_scheduled_day_titile[]={"Jours programm�s"/*"jours planifi�s"*/,"Scheduled days","giorni programmati","programmierte Tage","d�as programados"};
const unsigned char *string_scheduler_title[]={"Planification","Scheduler","pianificazione","Planer","planificaci�n"};
   char *string_extended_mode_title[]={"Mode �tendu","Extended mode","modalit� estesa","Erweiterter Modus","modo ampliado"};
  char *string_extended_state_title[]={"Etat",  	"State",	"stato",	"Status",	"estado"};
const unsigned char *string_extended_mode_state_title[]={"Etat du mode �tendu","Extended mode state","stato della","Status erweiterter","estado del"};
const unsigned char *string_extended_mode_state1_title[]={" "," ","modalit� estesa","Modus","modo ampliado"};
const unsigned char *string_extended_mode_function_title[]={"Mode de","Extended function mode","modalit� di","Erweiterter","modo de "};
const unsigned char *string_extended_mode_function1_title[]={"fonctionnement �tendu"," ","funzionamento estesa","Betriebsmodus","funcionamiento ampliado"};
const unsigned char *string_extended_mode_standbyLevel_title[]={"Niveau de ","Extended Standby level","livello di ","Erweiterte ","nivel de "};
const unsigned char *string_extended_mode_standbyLevel1_title[]={"veille �tendu"," ","standby esteso","Standby-Stufe"," espera ampliado"};


const unsigned char *string_extended_mode_standbyDelay_title[]={"Temps de ","Extended Standby delay","intervallo di ","Erweiterte ","tiempo de"};
const unsigned char *string_extended_mode_standbyDelay1_title[]={"veille �tendu"," "," standby esteso","Standby-Dauer","espera ampliado"};

const unsigned char *strings_extended_on_level_titile[] ={"Niveau d'allumage �tendu"/*�tendu Niveau d'allumage*/,"Extended on level","livello di","Erweiterte","nivel de "};
const unsigned char *strings_extended_on_level1_titile[] ={" "/*�tendu Niveau d'allumage*/," "," accensione esteso","Einschaltstufe","encendido ampliado"};



const unsigned char *extended_mode_state[]={"Etat du mode �tendu",	"Extended mode state",	"stato della modalita estesa",	"Status erweiterter Modus",	"estado del modo ampliado"};

const unsigned char *extended_mode_function[]={"Mode de fonctionnement �tendu",	"Extended function mode",	"modalita di funzionamento estesa",	"Erweiterter Betriebsmodus",	"modo de funcionamiento ampliado"};

const unsigned char *extended_mode_sl[]={"Niveau de veille �tendu",	"Extended standby level",	"livello di standby esteso",	"Erweiterte Standby-Stufe",	"nivel de espera ampliado"};
const unsigned char *extended_mode_sd[]={"Temps de veille �tendu",	"Extended standby delay",	"intervallo di standby esteso",	"Erweiterte Standby-Dauer",	"tiempo de espera ampliado"};
const unsigned char *extended_mode_ol[]={"Niveau d'allumage �tendu",	"Extended on level",	"livello di accensione esteso"	,"Erweiterte Einschaltstufe","nivel de encendido ampliado"};



//"Temps de veille",          "Standby delay",            "Timer di standby",         "Retardo de espera",        "Standby-Verz�gerung", 
//"Niveau de veille",         "Standby level",            "Livello di standby",       "Nivel de espera",          "Standby-Stufe",  