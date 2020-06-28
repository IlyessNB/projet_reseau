#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <glib.h>


  typedef struct {
    char* buffer;
    size_t taille;
  }buff;

  buff *init_buff(size_t len){
    buff *tab=malloc(sizeof(buff));
    tab -> buffer = malloc(sizeof(char*)*len);
    tab->taille = len;
    return tab;
  }

  // typedef struct{
  //   uint8_t type;
  //   uint8_t length;
  //   char *body;
  // }tlv;

  // typedef struct{
  //   uint8_t type;
  //   uint8_t length;
  //   uint16_t time_out;
  //   char* id;
  //   char* secret;
  //   char* data;
  // }tlv;

  // typedef struct{
  //   uint8_t magic;
  //   uint8_t version;
  //   uint16_t body_length;
  // }header;
  //
  // typedef struct{
  //   header* hd;
  //   tlv* t;
  // }datagram;
  //
  // void type_set(tlv* t, uint8_t new_type){
  //   t->type = new_type;
  // }
  //
  // void length_set(tlv* t,uint8_t new_length){
  //   t->length = new_length;
  // }
  //
  // void body_set(tlv *t, char *body){
  //   t->body = body;
  // }
  //
  // tlv* new_tlv(uint8_t new_type, uint8_t new_length, char *new_body){
  //   printf("--CREATION TLV--\n");
  //   tlv *t = NULL;
  //   t = malloc(sizeof(tlv));
  //   if(t == NULL)
  //     return NULL;
  //   memset(t, 0, sizeof(*t));
  //   type_set(t, new_type);
  //   length_set(t, new_length);
  //   body_set(t,new_body);
  //   return t;
  // }
  //
  // void magic_set(header* h){
  //   h->magic = 95;
  // }
  //
  // void version_set(header* h){
  //   h->version = 2;
  // }
  //
  // void body_length_set(header* h , uint16_t new_body_length){
  //   h->body_length = htons(new_body_length);
  // }
  //
  //
  // header* new_header(uint16_t new_body_length){
  //   printf("--CREATION HEADER DATAGRAM--\n");
  //   header* h = NULL;
  //   h = malloc(sizeof(header));
  //   if(h == NULL)
  //     return NULL;
  //   memset(h, 0, sizeof(*h));
  //   magic_set(h);
  //   version_set(h);
  //   body_length_set(h, new_body_length);
  //   return h;
  // }
  //
  // void header_get(datagram* d,header *h){
  //   d->hd = h;
  // }
  //
  // void tlv_get(datagram *d, tlv *t){
  //   d->t = t;
  // }


  // char* body_notify_ack(uint16_t new_mbz, char* new_tag, char* new_id){
  //   char* body_buffer = malloc(sizeof(new_mbz)+sizeof(new_tag)+sizeof(new_id));
  //   memcpy(body_buffer, &new_mbz, sizeof(new_mbz));
  //   memcpy(body_buffer+sizeof(new_mbz), new_tag, sizeof(new_tag));
  //   memcpy(body_buffer+sizeof(new_mbz)+sizeof(new_tag), new_id, sizeof(new_id));
  //   return body_buffer;
  // }
  //
  // char* body_dump(uint16_t new_mbz, char* new_tag){
  //   printf("*TLV DUMP\n");
  //   char* body_buffer = NULL;
  //   body_buffer = malloc (8);
  //   // printf("dump %lu\n",body_buffer->taille );
  //   memcpy(body_buffer, &new_mbz, sizeof(new_mbz));
  //   memcpy(body_buffer+sizeof(new_mbz), new_tag ,sizeof(new_tag));
  //   return body_buffer;
  // }
  //
  //
  //  char* body_publish(uint16_t time_out, char *new_id, char *new_secret, char* new_data){
  //   printf("*TLV PUBLISH*\n");
  //   char* body = NULL;
  //   body = malloc (sizeof(time)+sizeof(new_id)+sizeof(new_secret)+sizeof(new_data));
  //   memcpy(body, &time_out, sizeof(time_out));
  //   memcpy(body+sizeof(time_out), new_id ,sizeof(new_id));
  //   memcpy(body+sizeof(time_out)+sizeof(new_id), new_secret, sizeof(new_secret));
  //   memcpy(body+sizeof(time_out)+sizeof(new_id)+sizeof(new_secret), new_data, strlen(new_data));
  //   return body;
  // }
  //
  // datagram* new_datagram(uint8_t type){
  //   printf("CREACTION DATAGRAM\n");
  //   datagram *d = malloc(sizeof(datagram));
  //   if(d == NULL)
  //     return NULL;
  //   char *body_buffer=NULL;
  //   uint16_t time_out = 1;
  //   char *data= "I";
  //   char *id = "1";
  //   char *secret = "S";
  //   uint8_t length = 0;
  //
  //   if(type == 4){
  //     printf("ICI\n" );
  //     length = sizeof(time_out)+sizeof(id)+sizeof(secret)+strlen(data);
  //     body_buffer= malloc(length);
  //     body_buffer = body_publish( time_out, id, secret, data);
  //   }else if (type == 5) {
  //     printf("LA\n" );
  //     uint16_t mbz = 0;
  //     char *tag = "tag";
  //     length = 6;
  //     body_buffer = malloc(8);
  //     body_buffer = body_dump(mbz,tag);
  //   }
  //   if (type == 3) {
  //     printf(" OU LA\n" );
  //     char *body_buffer= malloc(sizeof(*body_buffer));
  //     body_buffer = body_notify_ack(0,"TAG","1");
  //   }
  //   // body = malloc(length);
  //   uint16_t body_length = sizeof(type)+length+sizeof(length);
  //   uint16_t length_bi = htons (body_length);
  //   printf("sizeof length:::::::::::: %d\n", length);
  //   printf("sizeof type ----66---------->%d\n", body_length);
  //   d->t = new_tlv(type, length, body_buffer);
  //   d->hd = new_header(body_length+4);
  //   return d;
  // }



  buff* creer_header(uint8_t type, uint16_t time) {
    // buff *p_buffer = NULL;
    uint8_t magic = 95;
    uint8_t version = 2;
    buff* p_buffer = NULL;
  //
  // // PUBISH
  if (type == 4) {
    time_t time_out = time;
    uint16_t time_bi = htons(time_out);
    char *id = "011011";
    char *secret = "Secret7";
    char data [237]= "Tu bluff mortoni..";
    uint8_t length = sizeof(uint16_t) + sizeof(char*) + sizeof(char*) + sizeof(char)*strlen(data);
    uint16_t body_length = sizeof(length) + sizeof(type) + length;
    uint16_t length_bi = htons (body_length);
    p_buffer = init_buff(body_length+4);
    memcpy(p_buffer->buffer, &magic, sizeof(uint8_t));
    memcpy(p_buffer->buffer+1, &version, sizeof(uint8_t));
    memcpy(p_buffer->buffer+2, &length_bi, sizeof(uint16_t));
    memcpy(p_buffer->buffer+4, &type, sizeof(uint8_t));
    memcpy(p_buffer->buffer+5, &length, sizeof(uint8_t));
    memcpy(p_buffer->buffer+6, &time_out, sizeof(uint16_t));
    memcpy(p_buffer->buffer+8, id, sizeof(char*));
    memcpy(p_buffer->buffer+16, secret, sizeof(char*));
    memcpy(p_buffer->buffer+24, data, sizeof(char)*strlen(data));
  }else if (type == 5) {
    // DUMP
    char *tag = "TAG";
    uint8_t length = 6;
    uint16_t mbz =0;
    uint16_t body_length = sizeof(type) + sizeof(mbz) + sizeof(char)*4 + sizeof(length);
    uint16_t length_bi = htons(body_length);
    p_buffer = init_buff(body_length+4);
    memcpy(p_buffer->buffer, &magic, sizeof(uint8_t));
    memcpy(p_buffer->buffer+sizeof(uint8_t), &version, sizeof(uint8_t));
    memcpy(p_buffer->buffer +sizeof(uint8_t) + sizeof(uint8_t), &length_bi, sizeof(length_bi));
    memcpy(p_buffer->buffer +sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint16_t), &type, sizeof(uint8_t));
    memcpy(p_buffer->buffer +sizeof(uint8_t) + sizeof(uint8_t) + sizeof(length_bi) + sizeof(uint8_t), &length, sizeof(uint8_t));
    memcpy(p_buffer->buffer +sizeof(uint8_t) + sizeof(uint8_t) + sizeof(length_bi) + sizeof(uint8_t) + sizeof(uint8_t), &mbz, sizeof(uint16_t));
    memcpy(p_buffer->buffer +sizeof(uint8_t) + sizeof(uint8_t) + sizeof(length_bi) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint16_t), tag, sizeof(char)*4);

  }else if (type == 8) {
    uint16_t body_length = 0;
    p_buffer = init_buff(body_length+4);
    memcpy(p_buffer->buffer, &magic, sizeof(uint8_t));
    memcpy(p_buffer->buffer+sizeof(uint8_t), &version, sizeof(uint8_t));
    memcpy(p_buffer->buffer +sizeof(uint8_t) + sizeof(uint8_t), &body_length, sizeof(uint16_t));
  }






  //NOTIFY ACK
    // uint8_t type = 3;
    // uint8_t mbz = 0;
    // uint16_t body_length = sizeof(type);
    // uint16_t length_bi = htons (body_length);
    // buff *p_buffer = init_buff(1024);
    // memcpy(p_buffer->buffer, &magic, sizeof(magic));
    // memcpy(p_buffer->buffer + sizeof(magic), &version, sizeof(version));
    // memcpy(p_buffer->buffer + sizeof(magic) + sizeof(version), &length_bi, sizeof(length_bi));
  //
  //   printf("magic %d\n", (uint8_t)*(p_buffer->buffer));
  //   printf("version %d\n", (uint8_t)*(p_buffer->buffer +1));
  //   printf("body_length%d\n", (uint8_t)*(p_buffer->buffer +2));
  //   printf("type %d\n", (uint8_t)*(p_buffer->buffer +4));
  //   printf("length %d\n", (uint8_t)*(p_buffer->buffer +5));
    return p_buffer;
  }

  buff* notify_ack(char* reply){
    uint8_t magic = 95;
    uint8_t version = 2;
    uint8_t type = 3;
    uint8_t length = sizeof(uint16_t) + sizeof(char)*4 + sizeof(char*);
    uint16_t mbz =0;
    char *tag = malloc(sizeof(char)*4);
    tag = reply+8;
    char *id = malloc(sizeof(char*));
    id = reply+12;
    uint16_t body_length = sizeof(uint8_t) +  length + sizeof(uint8_t);
    uint16_t length_bi = htons(body_length);
    buff *p_buffer = init_buff(body_length+4);
    memcpy(p_buffer->buffer, &magic, sizeof(uint8_t));
    memcpy(p_buffer->buffer+sizeof(uint8_t), &version, sizeof(uint8_t));
    memcpy(p_buffer->buffer + sizeof(uint8_t) + sizeof(uint8_t), &length_bi, sizeof(uint16_t));
    memcpy(p_buffer->buffer + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(length_bi), &type, sizeof(uint8_t));
    memcpy(p_buffer->buffer + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(length_bi) + sizeof(uint8_t), &length, sizeof(uint8_t));
    memcpy(p_buffer->buffer + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(length_bi) + sizeof(uint8_t) + sizeof(uint8_t), &mbz, sizeof(uint16_t));
    memcpy(p_buffer->buffer + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(length_bi) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint16_t), tag, sizeof(char)*4);
    memcpy(p_buffer->buffer + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(length_bi) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint16_t) + sizeof(char)*4,  id, sizeof(char*));
    return p_buffer;
  }

  void afficher_tab_recu(GList *list){
    if(g_list_length(list) > 0){
      printf("---------------TAB RECU-------------\n");
      buff *reply = init_buff(1024);
      GList *l = NULL;
      for ( l = list ; l != NULL; l = l->next ) {
        reply->buffer = l->data;
        printf("|Time out :%d\n", (uint8_t)*(reply->buffer+6));
        printf("|Id :%s\n", reply->buffer+12);
        printf("|Data :\n|%s\n\n", reply->buffer+20);
      }
      printf("------------------------------------\n");
    }else {
      printf("TABLE VIDE\n");
    }
  }

  void afficher_tab_publish(GList *list){
    if(g_list_length(list) > 0){
      printf("---------------TAB PUBLISH-------------\n");
      buff *reply = init_buff(1024);
      GList *l = NULL;
      for ( l = list ; l != NULL; l = l->next ) {
        reply->buffer = l->data;
        printf("|Time out :%d\n", (uint8_t)*(reply->buffer+6));
        printf("|Id :%s\n", reply->buffer+8);
        printf("|Data :\n|%s\n\n", reply->buffer+24);
      }
      printf("------------------------------------\n");
    }else {
      printf("TABLE VIDE\n");
    }
  }

  //
  // buff* creer_notify_ack(buff* reply){
  //   uint8_t magic = 95;
  //   uint8_t version = 2;
  //
  //   char* tag;
  //   tag = malloc(4*sizeof(*tag));
  //   char* id = malloc(8*sizeof(char*));
  //   id = reply->buffer+12;
  //   uint16_t mbz = 0;
  //   printf("mbz %lu\n",sizeof(mbz) );
  //   printf("size id %lu\n",sizeof(id) );
  //   uint8_t type = 3;
  //   uint8_t length = sizeof(mbz) + sizeof(tag)+ sizeof(id);
  //   uint16_t body_length = sizeof(length) + sizeof(type) + length;
  //   uint16_t length_bi = htons (body_length);
  //   buff *p_buffer = init_buff(1024);
  //   memcpy(p_buffer->buffer, &magic, sizeof(magic));
  //   memcpy(p_buffer->buffer + sizeof(magic), &version, sizeof(version));
  //   memcpy(p_buffer->buffer + sizeof(magic) + sizeof(version), &length_bi, sizeof(length_bi));
  //   memcpy(p_buffer->buffer + sizeof(magic) + sizeof(version) + sizeof(length_bi), &type, sizeof(type));
  //   memcpy(p_buffer->buffer + sizeof(magic) + sizeof(version) + sizeof(length_bi) + sizeof(type), &length, sizeof(length));
  //   memcpy(p_buffer->buffer + sizeof(magic) + sizeof(version) + sizeof(length_bi) + sizeof(type) + sizeof(length), &mbz, sizeof(mbz));
  //   memcpy(p_buffer->buffer + sizeof(magic) + sizeof(version) + sizeof(length_bi) + sizeof(type) + sizeof(length) + sizeof(mbz), &tag, sizeof(tag));
  //   memcpy(p_buffer->buffer + sizeof(magic) + sizeof(version) + sizeof(length_bi) + sizeof(type) + sizeof(length) + sizeof(mbz) + sizeof(tag), &id, sizeof(id));
  //   printf("magic %d\n", (uint8_t)*(p_buffer->buffer));
  //   printf("version %d\n", (uint8_t)*(p_buffer->buffer +1));
  //   printf("body_length%d\n", (uint8_t)*(p_buffer->buffer +2));
  //   printf("type %d\n", (uint8_t)*(p_buffer->buffer +4));
  //   printf("length %d\n", (uint8_t)*(p_buffer->buffer +5));
  //   printf("tag %s\n",tag);
  //   printf("id %s\n", id);
  //   return p_buffer;
  //
  // }
  void afficher_paquet_recu(char* reply){
    printf("----------------------------------------------\n");
    if ((uint8_t)*(reply+4) == 2) {
      printf("|    TVL : --NOTIFY--\n");
    }else if ((uint8_t)*(reply+4) == 7) {
      printf("|    TVL : --DUMP ACK--\n");
    }else if ((uint8_t)*(reply+4) == 6) {
      printf("|    TLV : --WARNING--\n");
    }
    printf("|Magic: %d\n",(uint8_t)*(reply));
    printf("|Version: %d\n",(uint8_t)*(reply+1));
    printf("|Body length: %d\n", (uint8_t)*(reply+2));
    if((uint8_t)*(reply+4) == 6){
      printf("|Type : %d\n", (uint8_t)*(reply+4));
      printf("|Length :%d\n",(uint8_t)*(reply+5));
      printf("|Message :\n|%s\n\n", reply+6);
    }else if((uint8_t)*(reply+4) == 2){
      printf("|Type : %d\n", (uint8_t)*(reply+4));
      printf("|Length :%d\n",(uint8_t)*(reply+5));
      printf("|Time out :%ld\n", (time_t)*(reply+6));
      printf("|Tag :%s\n", reply+8);
      printf("|Id :%s\n", reply+12);
      printf("|Data :\n|%s\n\n", reply+20);
    }else if((uint8_t)*(reply+4) == 7){
      printf("|Type : %d\n", (uint8_t)*(reply+4));
      printf("|Length :%d\n", (uint8_t)*(reply+5));
      printf("|Mbz :%s\n", reply+6);
      printf("|Tag :%s\n\n", reply+8);
    }
    printf("----------------------------------------------\n\n");
  }

  void afficher_buff(uint8_t t, char *b){
    printf("----------------------------------------------\n");
    if ((uint8_t)*(b+4) == 4) {
      printf("|    TVL : --PUBLISH--\n");
    }else if ((uint8_t)*(b+4) == 5) {
      printf("|    TVL : --DUMP--\n");
    }else if ((uint8_t)*(b+4) == 3) {
      printf("|    TLV : --NOTIFY ACK--\n");
    }
    printf("|Magic envoyer: %d\n",(uint8_t)*(b));
    printf("|Version envoyer: %d\n",(uint8_t)*(b+1));
    printf("|Body length envoyer:%d\n", (uint8_t)*(b+2));
    if(t == 4){
      printf("|Type envoyer :%d\n",(uint8_t)*(b+4));
      printf("|Length envoyer :%d\n",(uint8_t)*(b+5));
      printf("|Time out envoyer :%ld\n",(time_t)*(b+6));
      printf("|Id envoyer :%s\n", b+8);
      printf("|Secret envoyer :%s\n", b+16);
      printf("|Data :\n%s\n\n",b+24);
    }else if(t == 5){
      printf("|Type envoyer :%d\n",(uint8_t)*(b+4));
      printf("|Length envoyer :%d\n",(uint8_t)*(b+5));
      printf("|Mbz envoyer: %d\n", (uint16_t)*(b+6));
      printf("|Tag envoyer:%s\n\n", b+8);
    }else if (t == 3) {
      printf("|Type envoyer :%d\n",(uint8_t)*(b+4));
      printf("|Length envoyer :%d\n",(uint8_t)*(b+5));
      printf("|MBZ envoyer :%d\n",(uint16_t)*(b+6));
      printf("|Tag envoyer :%s\n", b+8);
      printf("|ID envoyer :%s\n\n", b+12);
    }
    printf("----------------------------------------------\n\n");

  }



  int main(){
    //GETADDRINFO
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    struct addrinfo *res, *p;
    int rc,s;
    printf("Entrez l'adresse d'un serveur :\n");
    char ad[100] = "jch.irif.fr";
    scanf("%s", ad);
    rc = getaddrinfo(ad, "1212", &hints, &res);
    if(rc != 0) {
      fprintf(stderr, "Échec de résolution de noms : %s\n", gai_strerror(rc));
      exit(1);
    }
    for(p = res; p != NULL; p = p->ai_next) {
      s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
      if(s < 0) {
        close(s);
        continue;
      }
      rc = connect(s, p->ai_addr, p->ai_addrlen);
      if(rc >= 0){
        break;
      }
      close(s);
    }
    if (p == NULL) {
      fprintf(stderr, "La connection a échoué.\n");
      exit(1);

    }
    freeaddrinfo(res);


    //ENVOIE TLV DUMP
    buff *b = creer_header(5,(uint16_t)0);
    buff *b_notif = NULL;
    b_notif = init_buff(1024);
    ssize_t envoi;
    envoi= send(s, b->buffer , b->taille ,0);
    if (envoi <0){
      perror("ERREUR SENDTO");
      exit(0);
    }
    free(b);

    buff *reply = NULL;
    buff *reply_notif = NULL;
    ssize_t reception;
    GList *table_recu = NULL;
    GList *table_publish = NULL;
    //RECEPTION DUMP ACK
    reply = init_buff(1024);
    reception = recvfrom(s, reply->buffer , reply->taille, 0, NULL, NULL);
    if(reception < 0){
      perror("ERREUR RECVFROM");
      exit(1);
    }

    reply = NULL;
    reply = init_buff(1024);
    reception = recvfrom(s, reply->buffer , reply->taille, 0, NULL, NULL);
    if(reception < 0){
      perror("ERREUR RECVFROM");
      exit(1);
    }
    uint8_t test = (uint8_t)*(reply->buffer+4);
    if (test == 2) {
      table_recu = g_list_append(table_recu, reply->buffer);
      b = notify_ack(reply->buffer);
      envoi= send(s, b->buffer , b->taille ,0);
      if (envoi <0){
        perror("ERREUR SENDTO");
        exit(0);
      }
      free(b);
    }
    int sec = 30000000;
    buff* vide  = init_buff(1024);
    int fils2 = fork();
    switch (fils2) {
      case -1:
        perror("Erreur fork.");
        exit(0);
      case 0:
        while (1) {
          //ENVOIE PAQUET VIDE TOUTE LES 30 SEC
          vide = creer_header(8,0);
          envoi= send(s, vide->buffer , vide->taille ,0);
          if (envoi <0){
            perror("ERREUR SENDTO");
            exit(0);
          }
          usleep(sec);
          free(vide);
        }
        break;
      default:
        break;
    }
    int fils = fork();
    switch (fils) {
      case -1:
        perror("Erreur fork.");
        exit(0);
        //GESTION NOTIFY ET NOTIFY ACK DANS UNE BOUCLE
      case 0:
        while(1){
          reply_notif = NULL;
          reply_notif = init_buff(1024);
          reception = recvfrom(s, reply_notif->buffer, reply_notif->taille, 0 , NULL, NULL);
          if(reception < 0){
            perror("ERREUR RECVFROM");
            exit(1);
          }
          uint8_t test_type = (uint8_t)*(reply_notif->buffer+4);
          //GERER NOTIFY
          if (test_type == 2) {
            table_recu = g_list_append(table_recu, reply_notif->buffer);
            b_notif = notify_ack(reply_notif->buffer);
          //AFFICHER WARNING
          }else if( test_type == 6) {
            afficher_paquet_recu(reply_notif->buffer);
            b_notif = creer_header(8,(uint16_t)3);
          }
          envoi= send(s, b_notif->buffer , b->taille ,0);
          if (envoi <0){
            perror("ERREUR SENDTO");
            exit(0);
          }
          free(b_notif);
        }
        break;
        //INTERFACE CLIENT ET ENVOIE PUBLISH
      default:
        while (1) {
          printf("---MENU---\n");
          printf("1-Publier un message\n");
          printf("2-Afficher la table des données recues\n");
          printf("3-Afficher la table des données publiées\n");
          printf("4-Quitter le serveur\n");
          printf("Choisir l'option (entre 1 et 4)\n");
          int choix = 0;
          scanf("%d", &choix);
          switch (choix) {
            //PUBLISH
            case 1:
              printf("Choisir le time out (entre 1 et 20):\n");
              time_t time = 0;
              scanf("%ld", &time);
              b = creer_header(4, time);
              afficher_buff(4,b->buffer);
              envoi= send(s, b->buffer , b->taille ,0);
              if (envoi <0){
                perror("ERREUR SENDTO");
                exit(0);
              }
              table_publish = g_list_append(table_publish, b->buffer);
              free(b);
              break;
              //AFFICHER TABLE RECU
            case 2:
              afficher_tab_recu(table_recu);
              break;
              //AFFICHER TABLE PUBLISH
            case 3:
              afficher_tab_publish(table_publish);
              break;
              //QUITTER LE PROGRAMME
            case 4:
              kill(fils, SIGKILL);
              kill(fils2, SIGKILL);
              exit(0);
              break;
          }
        }

    }
    return 0;
  }
