# BOE-bot
Samling av kod till SSY331s BOE-bot uppgift.
BOE bot examinationen bestod av 4 uppgifter, varav de 2 mest avancerade är med i detta repository.
Uppgift 3 bestod av en Arduino UNO på två stycken servomotorer samt en kula där bak i formen av en liten robot på två hjul, längst fram satt två spröt monterade som kortslöt vardera krets mot jord
när de körde in i något. På detta sätt kunde botten identifiera om den kolliderat med något på höger eller vänster sida. Koden implementerar ett antal olika manövrar beroende på om den kolliderat
med något till vänster, höger eller rakt fram. Dessutom implemeterades en funktion för att detektera om botten kört fast i ett hörn. Det implementerades också en funktion baserad på cirkelsektor matematik för att
hålla koll på robotens riktning så att den inte vände om och körde tillbaka igen. Uppgiftens mål var att botten skulle kunna köra genom en bana med olika hinder samt väggar till vänster och höger på så kort tid som möjligt.

Uppgift 4 bestod av samma bot fast spröten hade blivit ersatta av en ultraljudssensor. Uppgiften var sedan att få botten att hålla kunna hålla ett specifikt avstånd till ett objekt såsom en hand eller vägg.
Om objektet sedan förflyttar sig framåt eller bakåt ska botten bibehålla avståndet. t.ex. genom att backa när objektet rör sig mot botten.
