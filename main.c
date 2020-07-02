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
#include <pthread.h>
#include "main.h"

//structure avec un buffer et sa taille
buff *init_buff(size_t len){
  buff *tab=malloc(sizeof(buff));
  tab -> buffer = malloc(sizeof(char*)*len);
  tab->taille = len;
  return tab;
}

//renvoie un id chr* random avec des 1 et des 0 de taille 7
char* rand_id(){
  char *id = NULL;
  int rand;
  srandom(time(NULL));
  id = malloc(sizeof(char*));
  int i;
  for (i = 0; i < 7; i++) {
    rand = random() % 2;
    if ( rand == 1){
      id[i] = '1';
    }else{
      id[i] = '0';
    }
  }
  return id;
}

//retourne une structure avec le paquet dans le buffer : publish, dump et paquet vide
buff* creer_header(uint8_t type, uint16_t time, char* da) {
  uint8_t magic = 95;
  uint8_t version = 2;
  buff* p_buffer = NULL;
// PUBISH
  if (type == 4) {
    time_t time_out = time;
    char *id = rand_id();
    char *secret = "Secret7";
    char *data =malloc(sizeof(char)*227);
    data =  da;
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
    uint8_t length = sizeof(uint16_t) + (sizeof(char)*4) ;
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
    //PAQUET VIDE
    uint16_t body_length = 0;
    p_buffer = init_buff(body_length+4);
    memcpy(p_buffer->buffer, &magic, sizeof(uint8_t));
    memcpy(p_buffer->buffer+sizeof(uint8_t), &version, sizeof(uint8_t));
    memcpy(p_buffer->buffer +sizeof(uint8_t) + sizeof(uint8_t), &body_length, sizeof(uint16_t));
  }
  return p_buffer;
}

//pareil mais pour notify ack c'est un fonction differente pcq on fait en fonction du buffer que le serveur envoi
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

//afficher la GList table recu si vide affiche TABLE RECU VIDE
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
    printf("TABLE RECU VIDE\n");
  }
}

//idem mais pour table publish
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
    printf("TABLE PUBLISH VIDE\n");
  }
}

//recoit buffer reply du serveur  en fonctino du type on affiche les infos
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

//idem mais avec ce que nous on envoi
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

//variables globals
GList *table_recu = NULL;
GList *table_publish = NULL;
int s;

// fonction qui boucle sur 1 pour recevoir les notify et envoyer notify ack
void* recvf_notify(void* vargp){
  buff* reply_notif;
  buff* b_notif;
  int reception,envoi;
  while(1){
    reply_notif = NULL;
    reply_notif = init_buff(1024);
    b_notif = init_buff(1024);
    reception = recvfrom(s, reply_notif->buffer, reply_notif->taille, 0 , NULL, NULL);
    if(reception < 0){
      perror("ERREUR RECVFROM");
      exit(1);
    }
    uint8_t test_type = (uint8_t)*(reply_notif->buffer+4);
    if (test_type == 2) {
      table_recu = g_list_append(table_recu, reply_notif->buffer);
      b_notif = notify_ack(reply_notif->buffer);
    }else if( test_type == 6) {
      afficher_paquet_recu(reply_notif->buffer);
      b_notif = creer_header(8,(uint16_t)3,"");
    }
    envoi= send(s, b_notif->buffer , b_notif->taille ,0);
    if (envoi <0){
      perror("ERREUR SENDTO");
      exit(0);
    }
    free(b_notif);
  }
}


int main(int argc, char const *argv[]) {
  //GETADDRINFO
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = 0;
  struct addrinfo *res, *p;
  int rc;
  //choix de l'address
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
  buff *b = creer_header(5,(uint16_t)0,"");
  ssize_t envoi;
  envoi= send(s, b->buffer , b->taille ,0);
  if (envoi <0){
    perror("ERREUR SENDTO");
    exit(0);
  }
  free(b);

  buff *reply = NULL;
  ssize_t reception;
  //reception DUMP ACK
  reply = init_buff(1024);
  reception = recvfrom(s, reply->buffer , reply->taille, 0, NULL, NULL);
  if(reception < 0){
    perror("ERREUR RECVFROM");
    exit(1);
  }

  //thread pour la reception des notify
  pthread_t thread;

  //processus fils pour l'association avec timer envoi toute les 30s
  int sec = 30000000;
  buff* vide  = init_buff(1024);
  int fils = fork();
  switch (fils) {
    case -1:
      perror("Erreur fork");
      exit(0);
    case 0:
      while (1) {
        vide = creer_header(8,0,"");
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

  //thread pour appel de la fonction recvf_notify
  if (pthread_create(&thread, NULL, recvf_notify, NULL) == -1) {
    perror("Erreur thread");
    exit(0);
  }
  //interface client
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
      case 1:
        printf("Taper le message que vous voulez envoyer(max 227 caract):\n");
        char mess[227] = "ca marche pas";
        size_t max = 227;
        // getline(&mess, &max,stdin);
        printf("Choisir le time out (entre 1 et 100):\n");
        time_t time = 0;
        scanf("%ld", &time);
        b = creer_header(4, time,mess);
        afficher_buff(4,b->buffer);
        envoi= send(s, b->buffer , b->taille ,0);
        if (envoi <0){
          perror("ERREUR SENDTO");
          exit(0);
        }
        table_publish = g_list_append(table_publish, b->buffer);
        free(b);
        break;
      case 2:
        afficher_tab_recu(table_recu);
        break;
      case 3:
        afficher_tab_publish(table_publish);
        break;
      case 4:
        kill(fils, SIGKILL);
        // pthread_exit(NULL);
        exit(0);
        break;
    }
  }
  return 0;
}
