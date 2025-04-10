#ifndef request_h
#define request_h

#include <stdbool.h>
#include <stdint.h>

/*
Je parseert requests gebaseerd op code die is geschreven op
basis van een ABNF definitie.

Wanneer je in een situatie zit met minder beperkingen (met
name, werkgeheugen) zou je van de ABNF elementen een boom
bouwen waarin je later gegevens op kan halen.

We kiezen nu voor een pragmatischer benadering: met de op
ABNF gebaseerde functies geef je een verwijzing naar een
`struct request` mee, zodat je daar relevante gegevens in
kan schrijven.

Onderstaande is een SUGGESTIE. Je bent vrij een andere
indeling te kiezen. Een eerdere variant is getest. In
onderstaande variant heb ik een aantal wijzigingen staan die
beter passen bij de structuur van de ABNF. Ik heb die
wijzigingen onvoldoende getest. Het kan zijn dat je alsnog
wat kleine zaken moet wijzigen.
*/

enum method {
  METHOD_UNRECOGNIZED,
  METHOD_GET,
  METHOD_POST,
  METHOD_PUT,
  METHOD_DELETE
};

enum target {
  CONFIG_MODE,
  CONFIG_CBUFFSIZE,
  SENSORS_1,
  SENSORS_2,
  SENSORS_1_AVG,
  SENSORS_2_AVG,
  SENSORS_1_STDEV,
  SENSORS_2_STDEV,
  SENSORS_1_ACTUAL,
  SENSORS_2_ACTUAL,
  ROOT,
  TARGET_UNRECOGNIZED
};

enum field { FIELD_UNRECOGNIZED, FIELD_CONTENT_LENGTH };

struct target_config {
  enum target target;
  const char value[20];
  enum method methods[2];
  void (*handler)(const char* body);
};

extern const struct target_config ENDPOINTS_CONFIG[11];

struct request {
  enum method method;
  enum target target;
  enum field field;
  char body[20];
};

#endif