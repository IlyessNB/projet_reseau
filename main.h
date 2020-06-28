#ifndef main_h
#define main_h

typedef struct {
  char* buffer;
  size_t taille;
}buff;

buff *init_buff(size_t len);

char* rand_id();

buff* creer_header(uint8_t type, uint16_t time);

buff* notify_ack(char* reply);

void afficher_tab_recu(GList *list);

void afficher_tab_publish(GList *list);

void afficher_paquet_recu(char* reply);

void afficher_buff(uint8_t t, char *b);

void* recvf_notify(void* vargp);

#endif
