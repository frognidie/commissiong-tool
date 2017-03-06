
#include "strings.h"

const unsigned char *strings[] = { \

#ifndef APP_BAES
                                                                                                                    // 2.4.0, add German
//  French                      English                     Italian                     Spanish                     German

/// Main page
    "Paramètres capteur",       "Sensor parameters",        "Parametri sensore",        "Parámetros captador",      "Sensorparameter",
    "Paramètres de base",       "Basic parameters",         "Parametri di base",        "Configuración básica",     "Basicparameter",           // 2.4.0
    "Mode avancé",              "Advanced mode",            "Modalità avanzata",        "Modo avanzado",            "Erweiterter Modus",
    "Fichiers",                 "Files",                    "Files",                    "Archivos",                 "Dateien",
    "PnL capteur",              "PnL sensor",               "Metodo PnL",               "Captador PnL",             "PNL-Sensor",               // 2.4.0, modify Italian translation
    "Test",                     "Test",                     "Test",                     "Test",                     "Testen",                   // 2.4.0
    "Config outils",            "Settings",                 "Impostazioni",             "Herramientas config",      "Einstellungen",

/// Sensor parameters
    "Date",                     "Date",                     "Data",                     "Fecha",                    "Datum",                    // 2.4.0
    "Temporisation",            "Time delay",               "Ritardo di tempo",         "Temporización",            "Zeitverzögerung",          // 2.4.0, modify Italian translation
    "Heure",                    "Hour",                     "Ora",                      "Hora",                     "Stunde",                   // 2.4.0

    "Sensibilité",              "Sensitivity",              "Sensibilità",              "Sensibilidad",             "Empfindlichkeit",
    "Bas",                      "Low",                      "Bassa",                    "Baja",                     "Niedrig",
    "Moyen",                    "Medium",                   "Media",                    "Media",                    "Mittel",
    "Haut",                     "High",                     "Alta",                     "Alta",                     "Hoch",
    "Très élevé",               "Very high",                "Massima",                  "Muy alta",                 "Sehr hoch",

    "Étalonnage",               "Calibration",              "Calibrazione",             "Calibrado",                "Kalibrierung",
    "Attention, ce paramètre ne peut être modifié qu'en association avec un luxmètre. Voir fiche technique. Voulez-vous continuer?", \
    "Be careful, this adjustment can only be modified in association with a luxmeter. Please report to technical datasheets. Are you sure to continue?", \
    "Attenzione, questo parametro deve essere modificato utilizzando un luxometro. Vedere la scheda tecnica. Volete continuare?", \
    "Ten cuidado, este parámetro debe ser cambiada usando un luxometro. Ver la ficha de datos. ¿Quiere continuar?", \
    "Seien Sie vorsichtig, kann diese Einstellung nur in Verbindung mit einem Luxmeter geändert werden. Bitte melden Sie technische Datenblätter. Sind Sie sicher, den Vorgang fortsetzen?",

    "Facteur lumière art.",     "Daylight factor",          "Fatt. luce artificiale",   "Factor de luminosidad",    "Faktor Tageslicht",        // 2.4.0, modify Italian translation
    "Facteur lumière (cen.)",   "Daylight factor (cen.)",   "Fatt. luce (cen.)",        "Daylight factor (cen.)",   "Faktor Tageslicht (zen.)", // 2.4.0, modify Italian translation
    "Facteur lumière nat.",     "Natural light factor",     "Fatt. luce naturale",      "Natural light factor",     "Faktor natürliches Licht", // 2.4.0, modify Italian translation
    "Niveau de luminosité",        "Daylight level",           "Livello illuminazione",    "Nivel de luz",             "Tageslichtintensität",     // 2.4.0, modify Italian translation

    "Seuil de luminosité",      "Daylight setpoint",        "Soglia di luminosità",     "Umbral de luminosidad",    "Sollwert für Tageslicht",
    "Seuil",                    "Setpoint",                 "Soglia",                   "Umbral",                   "Sollwert",

    "Système de détection",     "Detection scheme",         "Sistema di rilevazione",   "Sistema de detección",     "Detektionsschema",         // 2.4.0, modify Italian translation
    "Initial",                  "Initial",                  "Iniziale",                 "Inicial",                  "Erste Erkennung",
    "Maintenir",                "Maintain",                 "Mantenim",                 "Manteni",                  "Beibehalten",
    "Redéclench",               "Retrigger",                "Retrigger",                "Desencad",                 "Nachtriggern",             // 2.4.0, modify Italian translation
    "Désactiver",               "Disabled",                 "Disabilitato",             "Desactivado",              "Deaktiviert",
    "IR seul",                  "PIR only",                 "solo PIR",                 "PIR sólo",                 "Nur PIR",
    "US seul",                  "US only",                  "solo US",                  "US sólo",                  "Nur US",
    "IR et US",                 "PIR and US",               "PIR e US",                 "PIR y US",                 "PIR und US",
    "IR ou US",                 "PIR or US",                "PIR o US",                 "PIR o US",                 "PIR od. US",
    "HF seul",                  "HF only",                  "solo HF",                  "HF sólo",                  "Nur HF",
    "PIR et HF",                "PIR and HF",               "PIR e HF",                 "PIR y HF",                  "PIR und HF",
    "PIR ou HF",                "PIR or  HF",               "PIR o HF",                 "PIR o HF",                  "PIR od. HF",

    "Activer",                  "Enabled",                  "Abilitato",                "Activado",                 "Aktiviert",

    "Mode",                     "Mode",                     "Modalità funzionamento",   "Modo",                     "Modus",                    // 2.4.0, modify Italian translation
    "Auto on/off",              "Auto on/off",              "Auto",                     "Automático on/off",        "Autom. ein/aus",           // 2.4.0, modify Italian translation
    "Mode passage",             "Walkthrough",              "Walkthrough",              "Modo paso",                "Rundgang",                 // 2.4.0, modify Italian translation
    "Manuel on/Auto off",       "Manual on/Auto off",       "Eco",                      "Manual on/Auto off",       "Man. EIN/Auto. aus",       // 2.4.0, modify Italian translation
    "Manual on/off",            "Manual on/off",            "Manuale on/off",           "Manual on/off",            "Manuell EIN/aus",
    "Partiel on/Groupe off",    "Partial on/Group off",     "Parziale On/Auto Off",     "Parcial on/Grupo off",     "Teilw. EIN/Gruppe aus",    // 2.4.0, modify Italian translation
    "Anticipation",             "Anticipation",             "Anticipazione",            "Anticipación",             "Vorfreude",                // 2.3.0

    "Alerte",                   "Alert",                    "Allarme",                  "Alerta",                   "Aktiviert",                // 2.4.0, modify Italian translation
    "Acoustique",               "Acoustic",                 "Acustico",                 "Acústico",                 "Akustisch",

    "Régulation",               "Light regulation",         "Regolaz luminosità",       "Regulación",               "Lichtregulierung",         // To correct Italian translation ???
    "asservissement",           "loop type",                "tipo di controllo",        "control",                "Regelung",                 // 2.4.0, modify Italian translation
    "boucle fermée",            "close loop",               "anello cihiuso",               "bucle cerrado",               "geschloss. Regelkreis",
    "boucle ouverte",           "open loop",                "anello aperto",                "bucle abierto",                "offener Regelkreis",
    "Apport de lumière",        "Provision of light",       "Contributo di luce",       "Aporte de luz",            "Lichtbereitstellung",      // 2.4.0, modify Italian translation
    "Auto",                     "Auto",                     "Auto",                     "Auto",                     "Autom.",                   // 2.4.0

    "Référent",                 "Referent",                 "Referente",                "Referente",                "Referent",

    "Groupe",                   "Group",                    "Grouppo",                  "Grupo",                    "Gruppe",

    "Type",                     "Type",                     "Tipo",                     "Tipo",                     "Typ",

    "Etat capteur",             "Sensor status",            "Stato sensore",            "Sensor de estado",         "Sensorstatus",             // 2.4.0, modify Italian translation, nothing actually changed
    "charge",                   "load",                     "carico",    				"carga",                     "Last",                     // 2.4.0, modify Italian translation
    "ON",                       "ON",                       "ON",                       "ON",                       "EIN",                      // 2.4.0
    "OFF",                      "OFF",                      "OFF",                      "OFF",                      "AUS",                      // 2.4.0
    "actif",                    "active",                   "ON",                       "activo",                   "aktiv.",                   // 2.4.0, modify Italian translation
    "inactif",                  "inactive",                 "OFF",                      "inactivo",                 "deaktiv.",                 // 2.4.0, modify Italian translation
    "Oui",                      "Yes",                      "Sì",                       "Sí",                       "ja",
    "Non",                      "No",                       "No",                       "No",                       "kein",
    "Pas de",                   "No",                       "No",                       "No",                       "keine",
    "régulation",               "regulation",               "regolazione",              "regulación",               "Regulierung",              // 2.4.0, modify Italian translation, nothing actually changed
    "Régulation",               "Regulation",               "Regolazione",              "Regulación",               "Regulierung",              // 2.4.0, modify Italian translation, nothing actually changed
    "dérogation",               "derogation",               "derogazione",              "derogation",               "Abweichung",               // 2.4.0, modify Italian translation
    "Dérogation",               "Derogation",               "Derogazione",              "Derogation",               "Abweichung",               // 2.4.0, modify Italian translation
    "détection",                "detection",                "rilevazione",              "detección",                "Erkennung",                // 2.4.0, modify Italian translation
    "Détection",                "Detection",                "Rilevazione",              "Detección",                "Erkennung",                // 2.4.0, modify Italian translation
    "Mode passage",             "Walkthrough",              "Walkthrough",              "Modo paso",                "Rundgang",                 // 2.4.0, modify Italian translation
    "détection du soleil",      "sun detection",    "rilevazione luce sole",    "detección de luz solar",            "Sonnenerkennung",

    "Niveau de veille",         "Standby level",            "Livello di standby",       "Nivel de espera",          "Standby-Stufe",            // 2.4.0, modify Italian translation
    "Temps de veille",          "Standby delay",            "Timer di standby",         "Retardo de espera",        "Standby-Verzögerung",      // 2.4.0, modify Italian translation
    "Pas de veille",            "No standby",               "No attesa",                "No espera",                "Kein Standby",
    "Illimité",                 "Unlimited",                "Illimitato",               "Ilimitado",                "Unbegrenzt",
    "Standby mode",             "Standby mode",             "Standby",                  "Standby mode",             "Standby",                  // 2.4.0, modify Italian translation

    "Condition d'envoi",        "Send condition",           "Condizione di invio",      "Condiciones de entrega",   "Sendebedingung",
    "Cyclique",                 "Cyclical",                 "Ciclico",                  "Cíclico",                  "Zyklisch",
    "Sur changement",           "On change",                "Su cambiamento",           "En cuanto al cambio",      "Bei Änderung",
    "Que sur demande",          "On request only",          "Solo su richiesta",        "Sólo por encargo",         "Nur auf Anfrage",
    "Chang. ou cyclique",       "Change or cyclical",       "Cambiam. o ciclico",       "Cambio o cíclico",         "Änderung. od. zykl.",       // Customized by Nicholas
    "Zone d'insensibilité",     "Dead band",                "Zona d'insensibilità",     "Zona de insensibilidad",   "Unempfindl.-bereich",                // Too long
    "Tps max. entre 2 mes.",    "Cyclical sending delay",   "Tempo tra 2 messaggi",     "Tiempo entre mensajes",    "Max. Abstd. zw. 2 Nachr.",   // Customized by Nicholas
    "Bande passante",           "Bandwidth",                "Banda passante",           "Ancho de banda",           "Bandbreite",

    "Heure d'été",              "Summer hour",              "Ora legale",               "Horario de verano",        "Sommerzeit",            // 2.4.0
    "pas de choix",             "No choice",                "nessuna scelta",           "sin elección",             "keine Auswahl",                // 2.4.0    //the following 4 items must have fixed sequence and must be treated as one item in placement
    "Europe",                   "Europe",                   "Europe",                   "Europe",                   "Europe",                   // 2.4.0
    "USA",                      "USA",                      "USA",                      "USA",                      "USA",                      // 2.4.0
    "aucun",                     "None",                     "nessuno",                     "ninguno",              "kein",                     // 2.4.0
    "Début",                    "Start day",                "Inizio",                   "Comienzo",                 "starten Tag",              // 2.4.0
    "Fin",                      "End day",                  "Fine",                     "Final",                    "ende Tag",                 // 2.4.0

    "Pays",                     "Country",                  "Paese",                    "País",                     "Land",                     // 2.4.0
    "Apport de lumière actuel", "Current provision of light", "Lum. lampada in corso",  "Brillo lámpara actual",    "Aktuelle Lichtbereitstellung",     // Too long
		"fuseau horaire",								"Time zones",									"Fuso orario",							"Huso horario",								"Zeitzone",

/// 2.2.0
    "Fonction room control",    "Functions room control",   "Funzione room control",    "Funciones room control",   "Raumüberw.-funktionen",            // 2.4.0, modify Italian translation, nothing actually changed
    "Réinitialisation room",    "Reinitialize room",        "Reinizializza room",       "Reinicializar room",       "Raumüberw. neu einstellen",        // 2.4.0, modify Italian translation
    "Retour usine détecteur",   "Return to factory sensor", "Reinizializza sensore",    "Volver fábrica sensor",    "Sensor auf Werkseinst. zur.-setz.",        // 2.4.0, modify Italian translation

/// 2.2.0
    "Autres Paramètres",        "Other parameters",         "Altri parametri",          "Otros parámetros",         "Andere Parameter",
    "Retour buzzer",            "Buzzer feedback",          "Avviso acustico",          "Zumbador permiten",        "Summer-Rückmeldung",
    "Filtre secteur",           "Power line filter",        "Filtro alim.",             "Filtro del sector",        "Leitungsfilter",
    "Type de charge",           "Load type",                "Tipo di carico",           "Tipo de carga",            "Lasttyp",
    "capacitif forcé",          "forced capacitive",        "capacitiva forzata",        "capacitivo forzado",        "kapazitiv zwangsgeschaltet",
    "inductif forcé",           "forced inductive",         "induttiva forzata",         "inductivo forzado",         "induktiv zwangsgeschaltet",
    "capacitif auto",           "auto capacitive",          "capacitiva auto",          "capacitivo automático",          "kapazitiv Auto",
    "inductif auto",            "auto inductive",           "induttiva auto",           "inductivo automático",           "induktiv Auto",
    "DALI",                     "DALI",                     "DALI",                     "DALI",                     "DALI",
    "DSI",                      "DSI",                      "DSI",                      "DSI",                      "DSI",
    "LED",                      "LED",                      "LED",                      "LED",                      "LED",

/// Files
    "Sauvegarde",               "Save file",                "Salvare il file",          "Guardar archivo",          "Datei speichern",
    "Nouveau fichier ...",      "New file ...",             "Nuovo file ...",           "Nuevo archivo ...",        "Neue Datei ...",
    "Voulez-vous vraiment remplacer ce fichier?",   "Do you really want to replace this file?",     "Vuoi davvero sostituire questo file?",     "¿Quiere sustituir este archivo?",      "Wollen Sie wirklich, diese Datei zu ersetzen?",
    "Le fichier est enregistré avec succès.",       "The file is saved successfully.",              "Il file è stato salvato correttamente.",   "El archivo se ha guardado satisfactoriamente.",        "Die Datei wird erfolgreich gespeichert.",
    "fichiers standard",        "standard files",            "file standard",            "archivos estándar",        "Standarddateien",
    "Désolé, ce n'est pas possible avec les détecteurs BUS.",       "Sorry, it's not possible with BUS sensors.",   "Siamo spiacenti, questo non è possibile con i sensore BUS.",       "Lo siento, esto no es posible con los detectores BUS.",        "Leider ist es nicht möglich mit BUS-Sensoren.",

/// 2.2.0
    "Couloir_hall_Auto",        "Corridor_hall_Auto",       "Corridoio_hall_Auto",      "Corredor_hall_Auto",       "Gang autom.",
    "Couloir_hall_Manuel",      "Corridor_hall_Manual",     "Corridoio_hall_Manuale",   "Corredor_hall_Manual",     "Gang manuell",
    "Bureau_individuel_Auto",   "Small_office_Auto",        "Piccolo_ufficio_Auto",     "Pequeña_oficina_Auto",     "Kleines Büro autom.",
    "Bureau_individuel_Manuel", "Small_office_Manual",      "Piccolo_ufficio_Manuale",  "Pequeña_oficina_Manual",   "Kleines Büro manuell",
    "Salle_réunion_Auto",       "Meeting_room_Auto",        "Sala_riunioni_Auto",       "Sala_reuniones_Auto",      "Sitzungsraum autom.",
    "Salle_réunion_Manuel",     "Meeting_room_Manual",      "Sala_riunioni_Manuale",    "Sala_reuniones_Manual",    "Sitzungsraum manuell",
    "Salle_classe_Auto",        "Classroom_Auto",           "Classe_Auto",              "Salón_clases_Auto",        "Schulungsraum autom.",
    "Salle_classe_Manuel",      "Classroom_Manual",         "Classe_Manuale",           "Salón_clases_Manual",      "Schulungsraum manuell",
    "Open_space_Auto",          "Open_space_Auto",          "Open_space_Auto",          "Open_space_Auto",          "Großraumbüro autom.",
    "Open_space_Manuel",        "Open_space_Manual",        "Open_space_Manuale",       "Open_space_Manual",        "Großraumbüro manuell",
    "Toilettes_Auto",           "Toilet_Auto",              "Bagno_Auto",               "Inodoro_Auto",             "Toilette autom.",
    "Toilettes_Manuel",         "Toilet_Manual",            "Bagno_Manuale",            "Inodoro_Manual",           "Toilette manuell",

    "Attendez sous le capteur pendant l'envoi du fichier.",         "Wait under sensor during file sending.",       "Attendere sotto il sensore durante la spedizione dei file.",       "Esperar delante del detector mientras que se envía el archivo.",       "Warten Sie unter Sensor während Dateiübertragung.",
    "Voulez-vous supprimer ce fichier?",                            "Do you really want to delete this file?",      "Vuoi veramente eliminare questo file?",                            "¿Quiere suprimir este archivo?",                                       "Wollen Sie wirklich diese Datei löschen?",

/// PnL sensor
    "PnL groupe",               "PnL group",                "Raggruppamento PnL",       "PnL grupo",                "PNL-Gruppe",               // 2.4.0, modify Italian translation
    "Ajout périphérique",       "Add peripheral",           "Aggiungi periferica",      "Añadir periférico",        "Peripherie hinzufügen",
    "Learn",                    "Learn",                    "Learn",                    "Learn",                    "Lern",
    "Fin",                      "End",                      "Fine",                     "Fin",                      "Beend",
    "Supp",                     "Erase",                    "Elim",                     "Sup",                      "Lösch",
    "Groupe effacé",            "Delete group",             "Cancella gruppo",          "Grupo borrado",            "Gruppe löschen",

/// Settings
    "Langues",                  "Languages",                "Lingue",                   "Idiomas",                  "Sprachen",
    "Version",                  "Version",                  "Versione",                 "Versión",                  "Version",
    "Batterie",                 "Battery",                  "Batteria",                 "Batería",                  "Akku",

    "Niveau de batterie",       "Battery level",            "Livello batteria",         "Nivel de batería",         "Akkustand",
    "Batterie faible",          "Low battery",              "Batteria scarica",         "Batería baja",             "Batterie schwach",

#ifndef NO_APPSWITCH
    "Changer d'application",       "Switch applications",       "Cambia applicazione",       "Cambiar de aplicación",       "Die Anwendung wechseln",
    "Etes-vous sûr de vouloir passer à l'application Eclairage de Sécurité ?",  "Do you really want to switch to Emergency Lighting application?",  "Sei sicuro di voler passare all’applicazione Illuminazione di Emergenza?",  "¿Está seguro de que desea pasar a la aplicación Luminaria de emergencia?",  "Möchten Sie wirklich in die Sicherheitsleuchten-Anwendung wechseln?",
    "Application non trouvée",    "Application not found",    "Applicazione non trovata",    "No se ha encontrado la aplicación",    "Anwendung nicht gefunden",
    "Recommencez…",        "Please restart...",        "Si prega ricominciare…",        "Reinicie…",        "Bitte wiederholen…",
#endif  // #ifndef NO_APPSWITCH

/// Test
    "Test fonctionnement",      "Walk test",                "Walk test",                "Test de funcionamiento",   "Gehtest",
    "Fin temporisation",        "End time delay",           "Fine ritardo di tempo",    "Fin temporización",        "Endzeitverzögerung",       // 2.4.0, modify Italian translation
    "Etat initial",             "Initial state",            "Stato iniziale",           "Estado inicial",           "Anfangszustand",
    "Contrôle",                 "Control",                  "Controllo",                "Control",                  "Kontrolle",                // 2.3.0
    "Lumières",                 "Lights",                   "Luce",                     "Luz",                      "Beleuchtung",              // 2.3.0
    "Auxiliaires",              "Auxiliaries",              "Ausiliario",               "Auxiliar",                 "Hilfstruppen",             // 2.3.0

/// Read parameters
    "Attendez encore pendant la lecture ...",           "Wait still while reading ...",         "Attendere la fine della lettura ...",      "Siga esperando mientras que se realiza la lectura ...",        "Warten Sie immer noch beim Lesen ...",
    "Erreur lors de la lecture. Essayez à nouveau.",    "Error reading. Try again.",            "Errore di lettura. Riprova.",              "Error de lectura. Vuelva a intentarlo.",                       "Fehler beim Lesen. Erneut versuchen.",
    "Valeur de(s) paramètre(s) inconnue.",              "Unknown value for parameter(s).",      "Valore ignoto del/i parametro/i.",          "Valor de parámetro(s) desconocido",                              "Unbekannter Wert für den Parameter. ",
    "Paramètre(s) non enregistré(s).",                  "Parameter(s) not implemented.",        "Parametro/i non implementato/i.",            "Parámetro(s) no registrado(s)",                                "Parameter nicht implementiert",

/// Help message
    "Toutes les informations techniques sont disponibles sur www.legrandoc.com",    "All technical information is available on www.legrandoc.com",      "Tutte le informazioni tecniche sono disponibili su www.legrandoc.com",     "Toda la información técnica a su disposición en www.legrandoc.com",     "Technische Informationen erhalten Sie unter www.legrandoc.com",
//2.4.1
	"Messages / minute",		"Messages/minute",			"Messaggi/minuto",			"Mensajes / minuto",	"Mitteilungen/Minute",
	"Longitude",				"Longitude",				"Longitudine",				"Longitud",				"Längengrad",
	"Latitude",					"Latitude",					"Latitudine",				"Latitud",				"Breitengrad",
	"Lumière algorithmique",	"Algorithmic light",		"Luce algoritmica",			"Luz algorítmica",		"Algorithmisches Licht",
	"Digital",					"Digital",					"Digitale",					"Digital",				"Digital",
	"Centralisé",				"Centralized ",				"Centralizzato",			"Centralizado",			"Zentralgesteuert",
	"Identifiant d'appareil Zigbee",	"Device ID zigbee",	"Identificativo di apparecchio Zigbee",	"Identificador del aparato Zigbee",	"Zigbee Gerätekennung",
	"Micrologiciel contrôleur",	"Controller firmware",		"Firmware controller",		"Controlador de micro programa informático",	"Regler-Firmware",
	"Micrologiciel",			"Firmware",					"Firmware", 				"Micro programa informático",	"Firmware",
	"Micrologiciel carte fille",	"Daughter board firmware",	"Firmware scheda figlia",	"Tarjeta secundaria de programa informático",	"Tochterkarten-Firmware",
	"Heure d’été",				"Summertime",				"Ora legale",				"Horario de verano",	"Sommerzeit",
	"Début",					"Start",					"Inizio",					"Inicio",				"Anfang",
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











