const unsigned char *KNX_sensor_type_name[] = 
{
   "d�tecteur KNX luminosit� seule",  //French
   "KNX sensor daylight only", //English  
   "sensore KNX solo luminosit�", //Italian
   "detector KNX luminosidad solo", //Spanish
   "KNX Sensor nur Tageslicht", //German
   
   "d�tecteur KNX PIR seulement d�tection seulement",  //French
   "KNX sensor PIR only occupancy only", //English  
   "sensore KNX solo PIR solo rilevazione", //Italian
   "detector KNX PIR solo presencia solo", //Spanish
   "KNX nur PIR Sensor nur Pr�senzmelder", //German
   
   "d�tecteur KNX PIR seulement d�tection et luminosit�",  //French
   "KNX sensor PIR only occupancy and daylight", //English  
   "sensore KNX solo PIR rilevazione e luminosit�", //Italian
   "detector KNX PIR solo presencia y luminosidad", //Spanish
   "KNX nur PIR Sensor Pr�senzmelder und Tageslicht", //German
   
   "d�tecteur KNX esclave PIR seulement d�tection seulement",  //French
   "SLAVE KNX sensor PIR only occupancy only", //English  
   "sensore KNX slave solo PIR solo rilevazione", //Italian
   "detector KNX esclavo PIR solo presencia solo", //Spanish
   "KNX Slave nur PIR Sensor nur Pr�senzmelder", //German
   
   "d�tecteur KNX PIR et US d�tection seulement",  //French
   "KNX sensor PIR and US occupancy only", //English  
   "sensore KNX PIR e US solo rilevazione", //Italian
   "detector KNX PIR y US presencia solo", //Spanish
   "KNX PIR und US Sensor nur Pr�senzmelder", //German
    
   "d�tecteur KNX PIR et US d�tection et luminosit�",  //French
   "KNX sensor PIR and US occupancy and daylight", //English  
   "sensore KNX PIR e US rilevazione e luminosit�", //Italian
   "detector KNX PIR y US presencia y luminosidad", //Spanish
   "KNX PIR und US Sensor Pr�senzmelder und Tageslicht", //German
   
   "d�tecteur KNX esclave PIR et US seulement d�tection",  //French
   "SLAVE KNX sensor PIR and US occupancy only", //English  
   "sensore KNX slave PIR e US solo rilevazione", //Italian
   "detector KNX esclavo PIR y US presencia solo", //Spanish
   "KNX Slave PIR und US Sensor nur Pr�senzmelder", //German
};

const unsigned char *KNX_sensor_master_slave_menu_item_langeage_string[] = 
{
   "ma�tre/esclave",  //French
   "master/slave", //English
   "master/slave", //Italian
   "maestro/esclavo", //Spanish
   "Master/Slave", //German
};

const unsigned char *KNX_sensor_master_slave_title_langeage_string[] = 
{
   "Ma�tre esclave",  //French
   "Master Slave", //English  
   "Master slave", //Italian               
   "Maestro esclavo", //Spanish                     
   "Master Slave", //German
};

const unsigned char *KNX_sensor_master_langeage_string[] = 
{
   "Ma�tre",  //French  
   "Master", //English  
   "Master", //Italian               
   "Maestro", //Spanish                     
   "Master", //German
};

const unsigned char *KNX_sensor_slave_langeage_string[] = 
{
   "esclave",  //French  
   "Slave", //English  
   "slave", //Italian               
   "esclavo", //Spanish                     
   "Slave", //German
};



const unsigned char *DALI_sensor_type_name[] = {
  //f
  "D�tecteur DALI ma�tre PIR et US lumi�re naturelle uniquement",
  "D�tecteur DALI ma�tre PIR et US pr�sence et lumi�re naturelle",
  "D�tecteur DALI ma�tre PIR et US",
  "D�tecteur DALI esclave PIR et US pr�sence uniquement",
  "D�tecteur DALI ma�tre PIR uniquement pr�sence et lumi�re naturelle",
  "D�tecteur DALI ma�tre PIR uniquement pr�sence uniquement",
  "D�tecteur DALI esclave PIR uniquement pr�sence uniquement",
  "D�tecteur DALI ma�tre PIR uniquement lumi�re naturelle uniquement",
  
  //e
  " DALI sensor PIR and US master daylight only",
  "DALI sensor PIR and US master occupancy and daylight",
  "DALI sensor PIR and US master occupancy only",
  "DALI sensor PIR and US slave occupancy only",
  "DALI sensor PIR only master occupancy and daylight",
  "DALI sensor PIR only master occupancy only",
  "DALI sensor PIR only slave occupancy only",
  "DALI sensor PIR only master daylight only",
  //i
  "Rilevatore DALI master PIR e US, solo luce naturale",
  "Rilevatore DALI master PIR e US, presenza e luce naturale",
  "Rilevatore DALI master PIR e US",
  "Rilevatore DALI slave PIR e US, solo presenza",
  "Rilevatore DALI master solo PIR, presenza e luce naturale",
  "Rilevatore DALI master solo PIR, solo presenza",
  "Rilevatore DALI slave solo PIR, solo presenza",
  "Rilevatore DALI master solo PIR, solo luce naturale",
  //s
  "Detector DALI maestro PIR y US luz natural �nicamente",
  "Detector DALI maestro PIR y US presencia y luz natural",
  "Detector DALI maestro PIR y US",
  "Detector DALI esclavo PIR y US presencia �nicamente",
  "Detector DALI maestro PIR y US �nicamente presencia y luz natural",
  "Detector DALI maestro PIR y US �nicamente presencia �nicamente",
  "Detector DALI esclavo PIR y US �nicamente presencia �nicamente",
  "Detector DALI maestro PIR y US �nicamente luz natural �nicamente",
  //g
  
  "DALI Master-Sensor PIR und US, nur Tageslicht",
  "DALI Master-Sensor PIR und US, Pr�senz und Tageslicht",
  "DALI Master-Sensor PIR und US",
  "DALI Slave-Sensor PIR und US, nur Pr�senz",
  "DALI Master-Sensor, nur PIR, Pr�senz und Tageslicht",
  "DALI Master-Sensor, nur PIR, nur Pr�senz",
  "DALI Slave-Sensor, nur PIR, nur Pr�senz",
  "DALI Master-Sensor, nur PIR, nur Tageslicht",

}; 

const unsigned char *DALI_ballast_name[] = {"Ballast", 	"Ballast", 	"Alimentatore", 	"Balasto", 	"Vorschaltger�t"};
const unsigned char *DALI_sensor_name[] = {"D�tecteur", 	"Sensor", 	"Rilevatore", 	"Detector", 	"Sensor",};
const unsigned char *DALI_changeToSlave_name[] = {"Passer en esclave",	"Change to slave",	"Passare a slave",	"Pasar a esclavo",	"Auf Slave umschalten",};
const unsigned char *DALI_eraseDaliSys_name[] = {"Effacer syst�me DALI","Erase DALI system",	"Cancellare sistema DALI",	"Borrar sistema DALI",	"DALI System l�schen",};


const unsigned char *DALI_warning_name[]={"ATTENTION","WARNING","ATTENZIONE","ATENCI�N","ACHTUNG"};
const unsigned char *DALI_warning_long_name[]={
"Toutes les associations seront effac�es VOULEZ-VOUS CONTINUER?",

"All associations will be erased PLEASE CONFIRM",

"Tutte le associazioni saranno cancellate SEI SICURO DI VOLER CONTINUARE?",

"Todas las asociaciones ser�n borradas �DESEA CONTINUAR?",

"S�mtliche Verbindungen werden gel�scht M�CHTEN SIE FORTFAHREN?",
};


const unsigned char *DALI_erase_long_name[]=
{
"Effacer toutes les associations DALI VOULEZ-VOUS CONTINUER ?",

"Erase DALI associations PLEASE CONFIRM",

"Cancellare tutte le associazioni DALI SEI SICURO DI VOLER CONTINUARE?",

"Borrar todas las asociaciones DALI �DESEA CONTINUAR?",

"S�mtliche DALI Verbindungen l�schen M�CHTEN SIE FORTFAHREN?",
};




