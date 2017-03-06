
#include "strings.h"

const unsigned char *strings[] = { \

#ifndef APP_BAES
                                                                                                                    // 2.4.0, add German
//  French                      English                     Italian                     Spanish                     German

/// Main page
    "Param�tres capteur",       "Sensor parameters",        "Parametri sensore",        "Par�metros captador",      "Sensorparameter",
    "Param�tres de base",       "Basic parameters",         "Parametri di base",        "Configuraci�n b�sica",     "Basicparameter",           // 2.4.0
    "Mode avanc�",              "Advanced mode",            "Modalit� avanzata",        "Modo avanzado",            "Erweiterter Modus",
    "Fichiers",                 "Files",                    "Files",                    "Archivos",                 "Dateien",
    "PnL capteur",              "PnL sensor",               "Metodo PnL",               "Captador PnL",             "PNL-Sensor",               // 2.4.0, modify Italian translation
    "Test",                     "Test",                     "Test",                     "Test",                     "Testen",                   // 2.4.0
    "Config outils",            "Settings",                 "Impostazioni",             "Herramientas config",      "Einstellungen",

/// Sensor parameters
    "Date",                     "Date",                     "Data",                     "Fecha",                    "Datum",                    // 2.4.0
    "Temporisation",            "Time delay",               "Ritardo di tempo",         "Temporizaci�n",            "Zeitverz�gerung",          // 2.4.0, modify Italian translation
    "Heure",                    "Hour",                     "Ora",                      "Hora",                     "Stunde",                   // 2.4.0

    "Sensibilit�",              "Sensitivity",              "Sensibilit�",              "Sensibilidad",             "Empfindlichkeit",
    "Bas",                      "Low",                      "Bassa",                    "Baja",                     "Niedrig",
    "Moyen",                    "Medium",                   "Media",                    "Media",                    "Mittel",
    "Haut",                     "High",                     "Alta",                     "Alta",                     "Hoch",
    "Tr�s �lev�",               "Very high",                "Massima",                  "Muy alta",                 "Sehr hoch",

    "�talonnage",               "Calibration",              "Calibrazione",             "Calibrado",                "Kalibrierung",
    "Attention, ce param�tre ne peut �tre modifi� qu'en association avec un luxm�tre. Voir fiche technique. Voulez-vous continuer?", \
    "Be careful, this adjustment can only be modified in association with a luxmeter. Please report to technical datasheets. Are you sure to continue?", \
    "Attenzione, questo parametro deve essere modificato utilizzando un luxometro. Vedere la scheda tecnica. Volete continuare?", \
    "Ten cuidado, este par�metro debe ser cambiada usando un luxometro. Ver la ficha de datos. �Quiere continuar?", \
    "Seien Sie vorsichtig, kann diese Einstellung nur in Verbindung mit einem Luxmeter ge�ndert werden. Bitte melden Sie technische Datenbl�tter. Sind Sie sicher, den Vorgang fortsetzen?",

    "Facteur lumi�re art.",     "Daylight factor",          "Fatt. luce artificiale",   "Factor de luminosidad",    "Faktor Tageslicht",        // 2.4.0, modify Italian translation
    "Facteur lumi�re (cen.)",   "Daylight factor (cen.)",   "Fatt. luce (cen.)",        "Daylight factor (cen.)",   "Faktor Tageslicht (zen.)", // 2.4.0, modify Italian translation
    "Facteur lumi�re nat.",     "Natural light factor",     "Fatt. luce naturale",      "Natural light factor",     "Faktor nat�rliches Licht", // 2.4.0, modify Italian translation
    "Niveau de luminosit�",        "Daylight level",           "Livello illuminazione",    "Nivel de luz",             "Tageslichtintensit�t",     // 2.4.0, modify Italian translation

    "Seuil de luminosit�",      "Daylight setpoint",        "Soglia di luminosit�",     "Umbral de luminosidad",    "Sollwert f�r Tageslicht",
    "Seuil",                    "Setpoint",                 "Soglia",                   "Umbral",                   "Sollwert",

    "Syst�me de d�tection",     "Detection scheme",         "Sistema di rilevazione",   "Sistema de detecci�n",     "Detektionsschema",         // 2.4.0, modify Italian translation
    "Initial",                  "Initial",                  "Iniziale",                 "Inicial",                  "Erste Erkennung",
    "Maintenir",                "Maintain",                 "Mantenim",                 "Manteni",                  "Beibehalten",
    "Red�clench",               "Retrigger",                "Retrigger",                "Desencad",                 "Nachtriggern",             // 2.4.0, modify Italian translation
    "D�sactiver",               "Disabled",                 "Disabilitato",             "Desactivado",              "Deaktiviert",
    "IR seul",                  "PIR only",                 "solo PIR",                 "PIR s�lo",                 "Nur PIR",
    "US seul",                  "US only",                  "solo US",                  "US s�lo",                  "Nur US",
    "IR et US",                 "PIR and US",               "PIR e US",                 "PIR y US",                 "PIR und US",
    "IR ou US",                 "PIR or US",                "PIR o US",                 "PIR o US",                 "PIR od. US",
    "HF seul",                  "HF only",                  "solo HF",                  "HF s�lo",                  "Nur HF",
    "PIR et HF",                "PIR and HF",               "PIR e HF",                 "PIR y HF",                  "PIR und HF",
    "PIR ou HF",                "PIR or  HF",               "PIR o HF",                 "PIR o HF",                  "PIR od. HF",

    "Activer",                  "Enabled",                  "Abilitato",                "Activado",                 "Aktiviert",

    "Mode",                     "Mode",                     "Modalit� funzionamento",   "Modo",                     "Modus",                    // 2.4.0, modify Italian translation
    "Auto on/off",              "Auto on/off",              "Auto",                     "Autom�tico on/off",        "Autom. ein/aus",           // 2.4.0, modify Italian translation
    "Mode passage",             "Walkthrough",              "Walkthrough",              "Modo paso",                "Rundgang",                 // 2.4.0, modify Italian translation
    "Manuel on/Auto off",       "Manual on/Auto off",       "Eco",                      "Manual on/Auto off",       "Man. EIN/Auto. aus",       // 2.4.0, modify Italian translation
    "Manual on/off",            "Manual on/off",            "Manuale on/off",           "Manual on/off",            "Manuell EIN/aus",
    "Partiel on/Groupe off",    "Partial on/Group off",     "Parziale On/Auto Off",     "Parcial on/Grupo off",     "Teilw. EIN/Gruppe aus",    // 2.4.0, modify Italian translation
    "Anticipation",             "Anticipation",             "Anticipazione",            "Anticipaci�n",             "Vorfreude",                // 2.3.0

    "Alerte",                   "Alert",                    "Allarme",                  "Alerta",                   "Aktiviert",                // 2.4.0, modify Italian translation
    "Acoustique",               "Acoustic",                 "Acustico",                 "Ac�stico",                 "Akustisch",

    "R�gulation",               "Light regulation",         "Regolaz luminosit�",       "Regulaci�n",               "Lichtregulierung",         // To correct Italian translation ???
    "asservissement",           "loop type",                "tipo di controllo",        "control",                "Regelung",                 // 2.4.0, modify Italian translation
    "boucle ferm�e",            "close loop",               "anello cihiuso",               "bucle cerrado",               "geschloss. Regelkreis",
    "boucle ouverte",           "open loop",                "anello aperto",                "bucle abierto",                "offener Regelkreis",
    "Apport de lumi�re",        "Provision of light",       "Contributo di luce",       "Aporte de luz",            "Lichtbereitstellung",      // 2.4.0, modify Italian translation
    "Auto",                     "Auto",                     "Auto",                     "Auto",                     "Autom.",                   // 2.4.0

    "R�f�rent",                 "Referent",                 "Referente",                "Referente",                "Referent",

    "Groupe",                   "Group",                    "Grouppo",                  "Grupo",                    "Gruppe",

    "Type",                     "Type",                     "Tipo",                     "Tipo",                     "Typ",

    "Etat capteur",             "Sensor status",            "Stato sensore",            "Sensor de estado",         "Sensorstatus",             // 2.4.0, modify Italian translation, nothing actually changed
    "charge",                   "load",                     "carico",    				"carga",                     "Last",                     // 2.4.0, modify Italian translation
    "ON",                       "ON",                       "ON",                       "ON",                       "EIN",                      // 2.4.0
    "OFF",                      "OFF",                      "OFF",                      "OFF",                      "AUS",                      // 2.4.0
    "actif",                    "active",                   "ON",                       "activo",                   "aktiv.",                   // 2.4.0, modify Italian translation
    "inactif",                  "inactive",                 "OFF",                      "inactivo",                 "deaktiv.",                 // 2.4.0, modify Italian translation
    "Oui",                      "Yes",                      "S�",                       "S�",                       "ja",
    "Non",                      "No",                       "No",                       "No",                       "kein",
    "Pas de",                   "No",                       "No",                       "No",                       "keine",
    "r�gulation",               "regulation",               "regolazione",              "regulaci�n",               "Regulierung",              // 2.4.0, modify Italian translation, nothing actually changed
    "R�gulation",               "Regulation",               "Regolazione",              "Regulaci�n",               "Regulierung",              // 2.4.0, modify Italian translation, nothing actually changed
    "d�rogation",               "derogation",               "derogazione",              "derogation",               "Abweichung",               // 2.4.0, modify Italian translation
    "D�rogation",               "Derogation",               "Derogazione",              "Derogation",               "Abweichung",               // 2.4.0, modify Italian translation
    "d�tection",                "detection",                "rilevazione",              "detecci�n",                "Erkennung",                // 2.4.0, modify Italian translation
    "D�tection",                "Detection",                "Rilevazione",              "Detecci�n",                "Erkennung",                // 2.4.0, modify Italian translation
    "Mode passage",             "Walkthrough",              "Walkthrough",              "Modo paso",                "Rundgang",                 // 2.4.0, modify Italian translation
    "d�tection du soleil",      "sun detection",    "rilevazione luce sole",    "detecci�n de luz solar",            "Sonnenerkennung",

    "Niveau de veille",         "Standby level",            "Livello di standby",       "Nivel de espera",          "Standby-Stufe",            // 2.4.0, modify Italian translation
    "Temps de veille",          "Standby delay",            "Timer di standby",         "Retardo de espera",        "Standby-Verz�gerung",      // 2.4.0, modify Italian translation
    "Pas de veille",            "No standby",               "No attesa",                "No espera",                "Kein Standby",
    "Illimit�",                 "Unlimited",                "Illimitato",               "Ilimitado",                "Unbegrenzt",
    "Standby mode",             "Standby mode",             "Standby",                  "Standby mode",             "Standby",                  // 2.4.0, modify Italian translation

    "Condition d'envoi",        "Send condition",           "Condizione di invio",      "Condiciones de entrega",   "Sendebedingung",
    "Cyclique",                 "Cyclical",                 "Ciclico",                  "C�clico",                  "Zyklisch",
    "Sur changement",           "On change",                "Su cambiamento",           "En cuanto al cambio",      "Bei �nderung",
    "Que sur demande",          "On request only",          "Solo su richiesta",        "S�lo por encargo",         "Nur auf Anfrage",
    "Chang. ou cyclique",       "Change or cyclical",       "Cambiam. o ciclico",       "Cambio o c�clico",         "�nderung. od. zykl.",       // Customized by Nicholas
    "Zone d'insensibilit�",     "Dead band",                "Zona d'insensibilit�",     "Zona de insensibilidad",   "Unempfindl.-bereich",                // Too long
    "Tps max. entre 2 mes.",    "Cyclical sending delay",   "Tempo tra 2 messaggi",     "Tiempo entre mensajes",    "Max. Abstd. zw. 2 Nachr.",   // Customized by Nicholas
    "Bande passante",           "Bandwidth",                "Banda passante",           "Ancho de banda",           "Bandbreite",

    "Heure d'�t�",              "Summer hour",              "Ora legale",               "Horario de verano",        "Sommerzeit",            // 2.4.0
    "pas de choix",             "No choice",                "nessuna scelta",           "sin elecci�n",             "keine Auswahl",                // 2.4.0    //the following 4 items must have fixed sequence and must be treated as one item in placement
    "Europe",                   "Europe",                   "Europe",                   "Europe",                   "Europe",                   // 2.4.0
    "USA",                      "USA",                      "USA",                      "USA",                      "USA",                      // 2.4.0
    "aucun",                     "None",                     "nessuno",                     "ninguno",              "kein",                     // 2.4.0
    "D�but",                    "Start day",                "Inizio",                   "Comienzo",                 "starten Tag",              // 2.4.0
    "Fin",                      "End day",                  "Fine",                     "Final",                    "ende Tag",                 // 2.4.0

    "Pays",                     "Country",                  "Paese",                    "Pa�s",                     "Land",                     // 2.4.0
    "Apport de lumi�re actuel", "Current provision of light", "Lum. lampada in corso",  "Brillo l�mpara actual",    "Aktuelle Lichtbereitstellung",     // Too long
		"fuseau horaire",								"Time zones",									"Fuso orario",							"Huso horario",								"Zeitzone",

/// 2.2.0
    "Fonction room control",    "Functions room control",   "Funzione room control",    "Funciones room control",   "Raum�berw.-funktionen",            // 2.4.0, modify Italian translation, nothing actually changed
    "R�initialisation room",    "Reinitialize room",        "Reinizializza room",       "Reinicializar room",       "Raum�berw. neu einstellen",        // 2.4.0, modify Italian translation
    "Retour usine d�tecteur",   "Return to factory sensor", "Reinizializza sensore",    "Volver f�brica sensor",    "Sensor auf Werkseinst. zur.-setz.",        // 2.4.0, modify Italian translation

/// 2.2.0
    "Autres Param�tres",        "Other parameters",         "Altri parametri",          "Otros par�metros",         "Andere Parameter",
    "Retour buzzer",            "Buzzer feedback",          "Avviso acustico",          "Zumbador permiten",        "Summer-R�ckmeldung",
    "Filtre secteur",           "Power line filter",        "Filtro alim.",             "Filtro del sector",        "Leitungsfilter",
    "Type de charge",           "Load type",                "Tipo di carico",           "Tipo de carga",            "Lasttyp",
    "capacitif forc�",          "forced capacitive",        "capacitiva forzata",        "capacitivo forzado",        "kapazitiv zwangsgeschaltet",
    "inductif forc�",           "forced inductive",         "induttiva forzata",         "inductivo forzado",         "induktiv zwangsgeschaltet",
    "capacitif auto",           "auto capacitive",          "capacitiva auto",          "capacitivo autom�tico",          "kapazitiv Auto",
    "inductif auto",            "auto inductive",           "induttiva auto",           "inductivo autom�tico",           "induktiv Auto",
    "DALI",                     "DALI",                     "DALI",                     "DALI",                     "DALI",
    "DSI",                      "DSI",                      "DSI",                      "DSI",                      "DSI",
    "LED",                      "LED",                      "LED",                      "LED",                      "LED",

/// Files
    "Sauvegarde",               "Save file",                "Salvare il file",          "Guardar archivo",          "Datei speichern",
    "Nouveau fichier ...",      "New file ...",             "Nuovo file ...",           "Nuevo archivo ...",        "Neue Datei ...",
    "Voulez-vous vraiment remplacer ce fichier?",   "Do you really want to replace this file?",     "Vuoi davvero sostituire questo file?",     "�Quiere sustituir este archivo?",      "Wollen Sie wirklich, diese Datei zu ersetzen?",
    "Le fichier est enregistr� avec succ�s.",       "The file is saved successfully.",              "Il file � stato salvato correttamente.",   "El archivo se ha guardado satisfactoriamente.",        "Die Datei wird erfolgreich gespeichert.",
    "fichiers standard",        "standard files",            "file standard",            "archivos est�ndar",        "Standarddateien",
    "D�sol�, ce n'est pas possible avec les d�tecteurs BUS.",       "Sorry, it's not possible with BUS sensors.",   "Siamo spiacenti, questo non � possibile con i sensore BUS.",       "Lo siento, esto no es posible con los detectores BUS.",        "Leider ist es nicht m�glich mit BUS-Sensoren.",

/// 2.2.0
    "Couloir_hall_Auto",        "Corridor_hall_Auto",       "Corridoio_hall_Auto",      "Corredor_hall_Auto",       "Gang autom.",
    "Couloir_hall_Manuel",      "Corridor_hall_Manual",     "Corridoio_hall_Manuale",   "Corredor_hall_Manual",     "Gang manuell",
    "Bureau_individuel_Auto",   "Small_office_Auto",        "Piccolo_ufficio_Auto",     "Peque�a_oficina_Auto",     "Kleines B�ro autom.",
    "Bureau_individuel_Manuel", "Small_office_Manual",      "Piccolo_ufficio_Manuale",  "Peque�a_oficina_Manual",   "Kleines B�ro manuell",
    "Salle_r�union_Auto",       "Meeting_room_Auto",        "Sala_riunioni_Auto",       "Sala_reuniones_Auto",      "Sitzungsraum autom.",
    "Salle_r�union_Manuel",     "Meeting_room_Manual",      "Sala_riunioni_Manuale",    "Sala_reuniones_Manual",    "Sitzungsraum manuell",
    "Salle_classe_Auto",        "Classroom_Auto",           "Classe_Auto",              "Sal�n_clases_Auto",        "Schulungsraum autom.",
    "Salle_classe_Manuel",      "Classroom_Manual",         "Classe_Manuale",           "Sal�n_clases_Manual",      "Schulungsraum manuell",
    "Open_space_Auto",          "Open_space_Auto",          "Open_space_Auto",          "Open_space_Auto",          "Gro�raumb�ro autom.",
    "Open_space_Manuel",        "Open_space_Manual",        "Open_space_Manuale",       "Open_space_Manual",        "Gro�raumb�ro manuell",
    "Toilettes_Auto",           "Toilet_Auto",              "Bagno_Auto",               "Inodoro_Auto",             "Toilette autom.",
    "Toilettes_Manuel",         "Toilet_Manual",            "Bagno_Manuale",            "Inodoro_Manual",           "Toilette manuell",

    "Attendez sous le capteur pendant l'envoi du fichier.",         "Wait under sensor during file sending.",       "Attendere sotto il sensore durante la spedizione dei file.",       "Esperar delante del detector mientras que se env�a el archivo.",       "Warten Sie unter Sensor w�hrend Datei�bertragung.",
    "Voulez-vous supprimer ce fichier?",                            "Do you really want to delete this file?",      "Vuoi veramente eliminare questo file?",                            "�Quiere suprimir este archivo?",                                       "Wollen Sie wirklich diese Datei l�schen?",

/// PnL sensor
    "PnL groupe",               "PnL group",                "Raggruppamento PnL",       "PnL grupo",                "PNL-Gruppe",               // 2.4.0, modify Italian translation
    "Ajout p�riph�rique",       "Add peripheral",           "Aggiungi periferica",      "A�adir perif�rico",        "Peripherie hinzuf�gen",
    "Learn",                    "Learn",                    "Learn",                    "Learn",                    "Lern",
    "Fin",                      "End",                      "Fine",                     "Fin",                      "Beend",
    "Supp",                     "Erase",                    "Elim",                     "Sup",                      "L�sch",
    "Groupe effac�",            "Delete group",             "Cancella gruppo",          "Grupo borrado",            "Gruppe l�schen",

/// Settings
    "Langues",                  "Languages",                "Lingue",                   "Idiomas",                  "Sprachen",
    "Version",                  "Version",                  "Versione",                 "Versi�n",                  "Version",
    "Batterie",                 "Battery",                  "Batteria",                 "Bater�a",                  "Akku",

    "Niveau de batterie",       "Battery level",            "Livello batteria",         "Nivel de bater�a",         "Akkustand",
    "Batterie faible",          "Low battery",              "Batteria scarica",         "Bater�a baja",             "Batterie schwach",

#ifndef NO_APPSWITCH
    "Changer d'application",       "Switch applications",       "Cambia applicazione",       "Cambiar de aplicaci�n",       "Die Anwendung wechseln",
    "Etes-vous s�r de vouloir passer � l'application Eclairage de S�curit� ?",  "Do you really want to switch to Emergency Lighting application?",  "Sei sicuro di voler passare all�applicazione Illuminazione di Emergenza?",  "�Est� seguro de que desea pasar a la aplicaci�n Luminaria de emergencia?",  "M�chten Sie wirklich in die Sicherheitsleuchten-Anwendung wechseln?",
    "Application non trouv�e",    "Application not found",    "Applicazione non trovata",    "No se ha encontrado la aplicaci�n",    "Anwendung nicht gefunden",
    "Recommencez�",        "Please restart...",        "Si prega ricominciare�",        "Reinicie�",        "Bitte wiederholen�",
#endif  // #ifndef NO_APPSWITCH

/// Test
    "Test fonctionnement",      "Walk test",                "Walk test",                "Test de funcionamiento",   "Gehtest",
    "Fin temporisation",        "End time delay",           "Fine ritardo di tempo",    "Fin temporizaci�n",        "Endzeitverz�gerung",       // 2.4.0, modify Italian translation
    "Etat initial",             "Initial state",            "Stato iniziale",           "Estado inicial",           "Anfangszustand",
    "Contr�le",                 "Control",                  "Controllo",                "Control",                  "Kontrolle",                // 2.3.0
    "Lumi�res",                 "Lights",                   "Luce",                     "Luz",                      "Beleuchtung",              // 2.3.0
    "Auxiliaires",              "Auxiliaries",              "Ausiliario",               "Auxiliar",                 "Hilfstruppen",             // 2.3.0

/// Read parameters
    "Attendez encore pendant la lecture ...",           "Wait still while reading ...",         "Attendere la fine della lettura ...",      "Siga esperando mientras que se realiza la lectura ...",        "Warten Sie immer noch beim Lesen ...",
    "Erreur lors de la lecture. Essayez � nouveau.",    "Error reading. Try again.",            "Errore di lettura. Riprova.",              "Error de lectura. Vuelva a intentarlo.",                       "Fehler beim Lesen. Erneut versuchen.",
    "Valeur de(s) param�tre(s) inconnue.",              "Unknown value for parameter(s).",      "Valore ignoto del/i parametro/i.",          "Valor de par�metro(s) desconocido",                              "Unbekannter Wert f�r den Parameter. ",
    "Param�tre(s) non enregistr�(s).",                  "Parameter(s) not implemented.",        "Parametro/i non implementato/i.",            "Par�metro(s) no registrado(s)",                                "Parameter nicht implementiert",

/// Help message
    "Toutes les informations techniques sont disponibles sur www.legrandoc.com",    "All technical information is available on www.legrandoc.com",      "Tutte le informazioni tecniche sono disponibili su www.legrandoc.com",     "Toda la informaci�n t�cnica a su disposici�n en www.legrandoc.com",     "Technische Informationen erhalten Sie unter www.legrandoc.com",
//2.4.1
	"Messages / minute",		"Messages/minute",			"Messaggi/minuto",			"Mensajes / minuto",	"Mitteilungen/Minute",
	"Longitude",				"Longitude",				"Longitudine",				"Longitud",				"L�ngengrad",
	"Latitude",					"Latitude",					"Latitudine",				"Latitud",				"Breitengrad",
	"Lumi�re algorithmique",	"Algorithmic light",		"Luce algoritmica",			"Luz algor�tmica",		"Algorithmisches Licht",
	"Digital",					"Digital",					"Digitale",					"Digital",				"Digital",
	"Centralis�",				"Centralized ",				"Centralizzato",			"Centralizado",			"Zentralgesteuert",
	"Identifiant d'appareil Zigbee",	"Device ID zigbee",	"Identificativo di apparecchio Zigbee",	"Identificador del aparato Zigbee",	"Zigbee Ger�tekennung",
	"Micrologiciel contr�leur",	"Controller firmware",		"Firmware controller",		"Controlador de micro programa inform�tico",	"Regler-Firmware",
	"Micrologiciel",			"Firmware",					"Firmware", 				"Micro programa inform�tico",	"Firmware",
	"Micrologiciel carte fille",	"Daughter board firmware",	"Firmware scheda figlia",	"Tarjeta secundaria de programa inform�tico",	"Tochterkarten-Firmware",
	"Heure d��t�",				"Summertime",				"Ora legale",				"Horario de verano",	"Sommerzeit",
	"D�but",					"Start",					"Inizio",					"Inicio",				"Anfang",
#else

/// Main page
    "ADRESSER ...",
    "TESTER ADRESSES...",
    "TEST ADRESSAGE",
    "EFFACER ADRESSE",

/// Settings
    "REGLAGES",
    "Version",
    "Batterie",
    "Niveau de batterie",
    "Batterie faible",

#ifndef NO_APPSWITCH
/// Switch application
    "Switch application",
    "Do you really want to switch to Lighting Management application?",
    "Application not found",
    "Please restart...",
#endif  // #ifndef NO_APPSWITCH

    "Oui",
    "Non",

#endif  // #ifndef APP_BAES

};











