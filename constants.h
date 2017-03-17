//Spotify data
const char* spotifyAcc = "1117954296";

//Bose data
char* boseIP = "";
const int bosePort = 8090;

//WiFi data
const char* ssid = "";
const char* password = "";

//Rotary pins
const int encPinA = D0;
const int encPinB = D1;
const int nextPin = D7;
const int lovePin = D2;

//Delay for unsupervised stream change
const int strChangeDelay = 750;

//Spotify streams
const String streams[] = {

  //DailyMx 1
  "spotify:station:user:1117954296:cluster:3uOCouLFR4bVx0XeiQJSbl", 
  //DailyMx 2
  "spotify:station:user:1117954296:cluster:69lxxQvsfAIoQbB20bEPFC", 
  //DailyMx 3
  "spotify:station:user:1117954296:cluster:1G1mX30GpUJqOr1QU2eBSs", 
  //DailyMx 4
  "spotify:station:user:1117954296:cluster:4m2iq2WEoxSFvOZd130HIQ", 
  //DailyMx 5
  "spotify:station:user:1117954296:cluster:5VTWoYYizcOY3uIKnxeCGI", 
  //DailyMx 6
  "spotify:station:user:1117954296:cluster:4lianjyuR1tqf6oUX8kjrZ", 
  
  //Radio - Yazoo
  "spotify:station:artist:1G1mX30GpUJqOr1QU2eBSs",                          
  
  //"Release Radar",
  "spotify:user:spotify:playlist:37i9dQZEVXbfgH0TtYCV1k",
  //"Discover Weekly",
  "spotify:user:spotify:playlist:37i9dQZEVXcKvwuegC9qpk",

  //"770 - Primeras canciones",
  "spotify:user:1117954296:playlist:53cVQEQs3AJg2apfGOJiSG",
  //"Descubrimientos",
  "spotify:user:1117954296:playlist:4L4k1nHskd2lAl4NVUKBru",
  //"Ambient a lo que da",
  "spotify:user:1117954296:playlist:7fa3RVGLtWCD4D9IA1JHQK",
  //"Italianismos",
  "spotify:user:1117954296:playlist:5xAjLuI5RT3Qi5TTwPs7lk",
  //"Me gustan, pero no tanto",
  "spotify:user:1117954296:playlist:2j2qjETKivkmOpbYgrToUT",
  //"Versiones",
  "spotify:user:1117954296:playlist:0eFP6ljmpb42dAvu9xFJq5",

  //"Michael Nyman - Composer Weekly #017",
  "spotify:user:composerweekly:playlist:6zQwQZB1fIcSxB6k6cq3Pd",
  //"John Adams - Composer Weekly #067",
  "spotify:user:composerweekly:playlist:4Qlh7bWzcNFTirntWLz3x6",
  //"Steve Reich - Composer Weekly #048",
  "spotify:user:composerweekly:playlist:5RrMA85R3u20gFP1w2aITo",
  //"Terry Riley - Composer Weekly #055",
  "spotify:user:composerweekly:playlist:2D9741chMzR8tTKDiCEBLc",
  //"This Is: Philip Glass",
  "spotify:user:spotify:playlist:0Qg1HXHsYxfWOmFClFpGuG",

  //"Indie Classical: Composed In 21st Century",
  "spotify:user:spotify:playlist:51lgGoMbd6Oto4rcq2UOqn",
  //"Classical X",
  "spotify:user:spotify:playlist:3Jc2HKlFVl1Cde9fyQNjhG",
  //"Next Classical",
  "spotify:user:spotify:playlist:1SBY0rHT0QjEtrzSNFzpMM",
  //"Minimalism",
  "spotify:user:spotify:playlist:5YuLa2rxwWrILo5KKHGCwX",
  //"The Sound of Ambient",
  "spotify:user:thesoundsofspotify:playlist:6CIyPj34GTpAkNwoWToLNT",

  //"Yo no hablo español",
  "spotify:user:fresus:playlist:0YJlDSrODSVEATX1XRIx9b",
  //"8-bit",
  "spotify:user:fresus:playlist:5C6N9k77yBk1gQm9UoPZEW",
  //"Frenchy but chic",
  "spotify:user:1126009907:playlist:2i6ScpsmEDixtzoeEjHXbY",
  //"Chicas y Rock&Roll",
  "spotify:user:adri:playlist:0fQQuFjmwbfunbjQPx5srp",
  //"Te imagino al volante de la nave espacial",
  "spotify:user:rblanco:playlist:3bFB51XgnQRxd7uma6L4uP",
  //"Fiesta Navideña Está Pasando Pinchada",
  "spotify:user:1126009907:playlist:3mG0QWlLS6wZuAnTSgdwx0",
  //"sing me to sleep",
  "spotify:user:el_farero:playlist:4Ybyor7xEzucpyqZLrECja",
  //"Mamarracha",
  "spotify:user:aegisthus:playlist:5v3jndUao1IXQdrwmKjGoK",
  //"New sounds for Kokuma",
  "spotify:user:therobot:playlist:1wSZWUQi2TbQgpgxOSeNzZ",
  //"Música electrónica para barbudos principiantes",
  "spotify:user:aegisthus:playlist:1vY5xwZqNERs6V2aslcZWv",
};

//Utility
const int strSize = sizeof(streams) / sizeof(char*);
