const unsigned char *KNX_sensor_type_name[] = 
{
   "détecteur KNX luminosité seule",  //French
   "KNX sensor daylight only", //English  
   "sensore KNX solo luminosità", //Italian
   "detector KNX luminosidad solo", //Spanish
   "KNX Sensor nur Tageslicht", //German
   
   "détecteur KNX PIR seulement détection seulement",  //French
   "KNX sensor PIR only occupancy only", //English  
   "sensore KNX solo PIR solo rilevazione", //Italian
   "detector KNX PIR solo presencia solo", //Spanish
   "KNX nur PIR Sensor nur Präsenzmelder", //German
   
   "détecteur KNX PIR seulement détection et luminosité",  //French
   "KNX sensor PIR only occupancy and daylight", //English  
   "sensore KNX solo PIR rilevazione e luminosità", //Italian
   "detector KNX PIR solo presencia y luminosidad", //Spanish
   "KNX nur PIR Sensor Präsenzmelder und Tageslicht", //German
   
   "détecteur KNX esclave PIR seulement détection seulement",  //French
   "SLAVE KNX sensor PIR only occupancy only", //English  
   "sensore KNX slave solo PIR solo rilevazione", //Italian
   "detector KNX esclavo PIR solo presencia solo", //Spanish
   "KNX Slave nur PIR Sensor nur Präsenzmelder", //German
   
   "détecteur KNX PIR et US détection seulement",  //French
   "KNX sensor PIR and US occupancy only", //English  
   "sensore KNX PIR e US solo rilevazione", //Italian
   "detector KNX PIR y US presencia solo", //Spanish
   "KNX PIR und US Sensor nur Präsenzmelder", //German
    
   "détecteur KNX PIR et US détection et luminosité",  //French
   "KNX sensor PIR and US occupancy and daylight", //English  
   "sensore KNX PIR e US rilevazione e luminosità", //Italian
   "detector KNX PIR y US presencia y luminosidad", //Spanish
   "KNX PIR und US Sensor Präsenzmelder und Tageslicht", //German
   
   "détecteur KNX esclave PIR et US seulement détection",  //French
   "SLAVE KNX sensor PIR and US occupancy only", //English  
   "sensore KNX slave PIR e US solo rilevazione", //Italian
   "detector KNX esclavo PIR y US presencia solo", //Spanish
   "KNX Slave PIR und US Sensor nur Präsenzmelder", //German
};

const unsigned char *KNX_sensor_master_slave_menu_item_langeage_string[] = 
{
   "maître/esclave",  //French
   "master/slave", //English
   "master/slave", //Italian
   "maestro/esclavo", //Spanish
   "Master/Slave", //German
};

const unsigned char *KNX_sensor_master_slave_title_langeage_string[] = 
{
   "Maître esclave",  //French
   "Master Slave", //English  
   "Master slave", //Italian               
   "Maestro esclavo", //Spanish                     
   "Master Slave", //German
};

const unsigned char *KNX_sensor_master_langeage_string[] = 
{
   "Maître",  //French  
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
  "Détecteur DALI maître PIR et US lumière naturelle uniquement",
  "Détecteur DALI maître PIR et US présence et lumière naturelle",
  "Détecteur DALI maître PIR et US",
  "Détecteur DALI esclave PIR et US présence uniquement",
  "Détecteur DALI maître PIR uniquement présence et lumière naturelle",
  "Détecteur DALI maître PIR uniquement présence uniquement",
  "Détecteur DALI esclave PIR uniquement présence uniquement",
  "Détecteur DALI maître PIR uniquement lumière naturelle uniquement",
  
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
  "Detector DALI maestro PIR y US luz natural únicamente",
  "Detector DALI maestro PIR y US presencia y luz natural",
  "Detector DALI maestro PIR y US",
  "Detector DALI esclavo PIR y US presencia únicamente",
  "Detector DALI maestro PIR y US únicamente presencia y luz natural",
  "Detector DALI maestro PIR y US únicamente presencia únicamente",
  "Detector DALI esclavo PIR y US únicamente presencia únicamente",
  "Detector DALI maestro PIR y US únicamente luz natural únicamente",
  //g
  
  "DALI Master-Sensor PIR und US, nur Tageslicht",
  "DALI Master-Sensor PIR und US, Präsenz und Tageslicht",
  "DALI Master-Sensor PIR und US",
  "DALI Slave-Sensor PIR und US, nur Präsenz",
  "DALI Master-Sensor, nur PIR, Präsenz und Tageslicht",
  "DALI Master-Sensor, nur PIR, nur Präsenz",
  "DALI Slave-Sensor, nur PIR, nur Präsenz",
  "DALI Master-Sensor, nur PIR, nur Tageslicht",

}; 

const unsigned char *DALI_ballast_name[] = {"Ballast", 	"Ballast", 	"Alimentatore", 	"Balasto", 	"Vorschaltgerät"};
const unsigned char *DALI_sensor_name[] = {"Détecteur", 	"Sensor", 	"Rilevatore", 	"Detector", 	"Sensor",};
const unsigned char *DALI_changeToSlave_name[] = {"Passer en esclave",	"Change to slave",	"Passare a slave",	"Pasar a esclavo",	"Auf Slave umschalten",};
const unsigned char *DALI_eraseDaliSys_name[] = {"Effacer système DALI","Erase DALI system",	"Cancellare sistema DALI",	"Borrar sistema DALI",	"DALI System löschen",};


const unsigned char *DALI_warning_name[]={"ATTENTION","WARNING","ATTENZIONE","ATENCIÓN","ACHTUNG"};
const unsigned char *DALI_warning_long_name[]={
"Toutes les associations seront effacées VOULEZ-VOUS CONTINUER?",

"All associations will be erased PLEASE CONFIRM",

"Tutte le associazioni saranno cancellate SEI SICURO DI VOLER CONTINUARE?",

"Todas las asociaciones serán borradas ¿DESEA CONTINUAR?",

"Sämtliche Verbindungen werden gelöscht MÖCHTEN SIE FORTFAHREN?",
};


const unsigned char *DALI_erase_long_name[]=
{
"Effacer toutes les associations DALI VOULEZ-VOUS CONTINUER ?",

"Erase DALI associations PLEASE CONFIRM",

"Cancellare tutte le associazioni DALI SEI SICURO DI VOLER CONTINUARE?",

"Borrar todas las asociaciones DALI ¿DESEA CONTINUAR?",

"Sämtliche DALI Verbindungen löschen MÖCHTEN SIE FORTFAHREN?",
};




