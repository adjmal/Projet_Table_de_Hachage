//BOURHANE
//ADJMAL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ********************************************************************************************
// Partie 1
// ********************************************************************************************

// ********************************************************************************************
// Type LISTE
// Implantation d'un type LISTE chainee dont chaque maillon
// contient un ELEMENT formé d'une cle de caracteres et d'un entier
// ********************************************************************************************

// Definition du type ELEMENT
typedef struct element ELEMENT;
struct element {
    char* cle;       // La cle de caracteres
    int valeur;      // Valeur associee a la cle
};

// Le type pour les maillons de la liste chainee
typedef struct maillon MAILLON;

// Un maillon contient un ELEMENT et l'adresse de son successeur

struct maillon{
    ELEMENT elem;
    MAILLON *succ;
};




// nouveauMaillon     :
// allocation memoire pour un nouveau maillon
// Retourne un pointeur sur un nouveau maillon
MAILLON* nouveauMaillon(){
    return (MAILLON*) malloc(sizeof(MAILLON));
}




// Le type LISTE : une liste est identifiee par l’adresse d’un maillon
typedef MAILLON *LISTE;

// liteVide : -> LISTE
// Retourne une liste vide, c'est à dire le pointeur NULL
LISTE listeVide(){
    return NULL;
}
// listeAjouter : ELEMENT x LISTE -> LISTE
// Ajoute un ELEMENT au debut d'une LISTE
// La liste resultat est reperee par l'adresse du nouveau maillon place au debut de l
LISTE listeAjouter(ELEMENT e, LISTE l){
    MAILLON *m = nouveauMaillon();
    m->elem = e;
    m->succ = l;
    return m;
}




// premier : LISTE -> ELEMENT
// Pre-cond : defini uniquement si l n'est pas vide
// On retourne le contenu du premier maillon de la liste
// La liste l n'est pas modifiee !
ELEMENT listePremier(LISTE l){
    if (l == NULL) {
        fprintf(stderr, "\npremier : erreur liste vide");
        abort();
    }
    return (l->elem);
}

// listeReste : LISTE -> LISTE
// Pre-cond : defini uniquement si l n'est pas vide
// Entree : une LISTE l
// Sortie : la liste l sans son premier maillon,
//          c'est a dire  une liste qui commence au deuxième maillon de l
LISTE listeReste(LISTE l){
    if (l == listeVide()) {
        fprintf(stderr, "\nreste : erreur lise vide");
        abort();
    }
    l = l->succ;
    return l;
}




// Longueur d'une liste
// Entree : une LISTE l
// Sortie : le nombre d'ELEMENTs dans l
//info: ATTENTION, cette fonction utiliser la notion de successeur des mallons pour determiner le nbr de successeur
int listeLongueur(LISTE l){
    int lg=0;
    while(l != NULL){
        lg++;
        l=listeReste(l);
    }
    return lg;
}

// Concatenation de deux listes
// Entree : deux LISTE l1 et l2
// Sortie : la liste resultat de la concatenation de l1 et l2
LISTE concat(LISTE l1, LISTE l2){
    MAILLON* p;
    if (l1 == NULL)
        return l2;
    p = l1;
    while (p->succ != NULL)
        p = p->succ;
    p->succ = l2;
    return l1;
}

// =========== A ECRIRE ================
// Affichage d'une liste
// Entree : une LISTE l
// Sortie : rien
// Effet de bord : affichage a l'ecran de la liste sous la forme :
// <(cle1, valeur>)(cle2, caleur2)...(clek, valeurk)>

void listeAfficher(LISTE l){
    LISTE p=l;
    printf("<");
    if(l==NULL){
        printf(">");
    }
    else{
        while(p->succ != NULL){
            printf("(%s,%d)",listePremier(p).cle,listePremier(p).valeur);
            p=p->succ;
        }
        printf("(%s,%d)>",listePremier(p).cle,listePremier(p).valeur);

    }
}

// =========== A ECRIRE ================
// Recherche d'un element dans une liste
// Entree : Une LISTE l et une cle
// Sortie : un pointeur sur le premier maillon qui contient un ELEMENT dont la cle est identique celle donnee
//          ou bien le pointeur NULL si aucun maillon ne contient la cle recherchee


MAILLON* listeChercher(LISTE l, char* cle){
  MAILLON* elementachercher = l;
  while (elementachercher!=NULL) {
    if (strcmp(elementachercher->elem.cle, cle)) {
      return elementachercher;
      }
      elementachercher=listeReste(elementachercher);
      if (elementachercher==NULL) {

        return NULL;
      }
    }
    return NULL;
  }

// =========== A ECRIRE ================
// LISTE listeSupprimer
// Suppression d'un maillon dans une liste
// Entree : une LISTE l et une cle
// Sortie : la liste l dans laquelle on a supprime le maillon contenant cle (s'il y en a un)
// Effet de bord : s'il y a un maillon avec la cle recherchee la memoire allouee a celui-ci est liberee

LISTE listeSupprimer(LISTE l, char* cle){

  MAILLON* destroy=listeChercher(l,cle);
  if(destroy==l){l=listeVide(); return l; }
    if(l == NULL) return NULL;
    if(l->elem.cle == cle){
        MAILLON* m = l->succ;
        free(l);
        m=listeSupprimer(m,cle);
        return m;
    }
    else{
        l->succ=listeSupprimer(l->succ,cle);
        return l;
    }

}
// =========== A ECRIRE ================
// listeFree : libere toute la memeoire allouee a une liste
// Entree : une LISTE
// Sortie : la liste vide NULL
// Effet de bord : liberation de toute la memeoire allouee aux maillons de la liste

LISTE listeFree(LISTE l){
    MAILLON* m = l;
    while(l!=NULL){
        m=l;
        l=l->succ;
        free(m);

    }
    return listeVide();
}

// ********************************************************************************************
// Partie 2
// ********************************************************************************************

// ********************************************************************************************
// Type HASHTABLE
// Implantation d'un type HASTTABLE pour representer une table de hachage
// par un tableau dynamique de listes chainees
// ********************************************************************************************

// Type pour les tables de hachage
// Taille par defaut d'une table de hachage
const int TAILLEDEFAUT = 1023;

typedef struct hashtable HASHTABLE;
struct hashtable {
    int taille;              // Taille de la table
    LISTE *table;            // Tableau dynamique de LISTEs
};




// Fonction de hachage pour une chaine de caracteres
// Entree : une chaine de caractere ch et la taille de la table de hachage
// Sortie : la valeur de hachage de la chaine ch
//          c'est à dire l'indice (compris entre 0 et taille-1 inclus )
//          auquel doit etre place la chaine dans la table de hachage

int hash(char* ch , int taille)
{
    int res = 0;            // Valeur calculee
    int i;

    for (i = 0; ch [i] != '\0'; i++) {
        res = res*256 + (unsigned int) ch[i];
        if (res >= taille)
            res %= taille;
    }
    return res;
}




// =========== A ECRIRE ================
// hashTableInit : Allocation et initialisation d'une table de hachage
// Entree : la taille de la table
// Sortie : une HASHTABLE initialisee avec un tableau pouvant contenir taille LISTEs
//          chaque liste etant initialisee avec une liste vide

HASHTABLE hashTableInit(int taille){
  HASHTABLE h;
  int i;
  h.taille=taille;
  h.table=NULL;
  h.table=malloc(h.taille * sizeof(LISTE));
  for (i = 0; i < taille; i++) {
    h.table[i]=listeVide();//remplissage des elements de la table par des listes vide
  }
  return h;//h est une HASHTABLE contenant un tableau de taille mis en argument et initialisé par des listes vide
}






// =========== A ECRIRE ================
// hashTableAfficher : Affichage complet d'une table de hachage
// Entree : une HASHTABLE ht
// Sortie : rien
// Effet de bord : affichage de la table sous la forme decrite ci-dessous
// (utiliser la fonction listeAfficher pour afficher chacune des listes)
//
// ** Contenu de la table de hachage **
// Indice 0 : <liste des elements>
// Indice 1 : <liste des elements>
// Indice 2 : <liste des elements>
//    ...

void hashTableAfficher(HASHTABLE ht){
    int i;
    printf("** Contenu de la table de hachage **\n");
    for(i=0;i<ht.taille;i++){
      printf("Indice %d :",i+1);
      listeAfficher(ht.table[i]);//affichage des listes suivant leur indice
      printf("\n");
    }
}




// =========== A ECRIRE ================
// hashTableChercher : Recherche d'un element dans la table de hachage
// Entree : une table ht et la cle recherchee
// Sortie : un pointeur sur le maillon contenant la cle si elle est trouvee
//          sinon le pointeur NULL
// Principe : Calculer la valeur de hachage de la cle; cette valeur donne l'indice de la  liste
//            ou doit se trouver l'element contenant la cle demandee.
//            Rechercher la cle dans la liste siutee a l'indice calcule.

MAILLON* hashTableChercher(HASHTABLE ht, char* cle){
    int i;
    int lol=hash(cle,ht.taille);//hachage de la cle donner en argument de la fonction
    for(i=0;i<ht.taille;i++){
        if(ht.table[lol]==ht.table[i]){
          if (listeChercher(ht.table[i],cle)) {
              return listeChercher(ht.table[i],cle);//pointeur du maillon contenant la clé

            }
        }
    }
    return NULL;//return null si aucun pointeur sur un maillon n'a été trouve

}




// =========== A ECRIRE ================
// hashTableInserer : Insertion d'un element dans la table
// Entree : une table ht, une cle ch
// Sortie : rien
// Effet de bord : insertion de l'element ch dans la table
// Principe :
//   si la cle ch est deja dans la table alors on incremente le nombre d'occurrences de l'ELEMENT correspondant
//   sinon on creer un nouvel ELEMENT qu'on insere dans la liste situee a l'indice de hachage


void hashTableInserer(HASHTABLE ht, char* cle){
  MAILLON* m = hashTableChercher(ht, cle);
  int i;
    if(m != NULL){
        m->elem.valeur++;//on augmente la valeur du nombre d'occurence tant que le maillon m est different de null
    }else{
        int index = hash(cle,ht.taille);
        char *nouveaumot = (char*)malloc(30*sizeof(char));
        for( i=0; i<29; i++){
            nouveaumot[i]=cle[i];
        }
        nouveaumot[29] = '\0';
        ELEMENT nouveauelement;
        nouveauelement.cle = nouveaumot;
        nouveauelement.valeur=1;
        LISTE l = listeVide();
        ht.table[index]=listeAjouter(nouveauelement,l);
    }
  }






// =========== A ECRIRE ================
// Suppression d'un element dans la table
// Entree : une table ht, une cle  ch
// Sortie : rien
// Effet de bord : suppression de l'element de cle ch dans la table
// Principe :ap
//   on supprime le maillon qui contient la cle dans la liste
//   situee, dans la table, a l'indice de hachage correspondant a la cle

void hashTableSupprimer(HASHTABLE ht, char* cle){
    int i=0;
    int hachp=hash(cle,ht.taille);
    ht.table[hachp]=listeSupprimer(ht.table[hachp],cle);
}




// =========== A ECRIRE ================
// hashTableVider : vidage de la table
// Entree : une HASHTABLE ht
// Sortie : rien
// Effet de bord : toutes les listes sont liberees
//   apres l'operation ht est une table vide

void hashTableVider(HASHTABLE ht){
    int i;
    for(i=0;i<ht.taille;i++){
      ht.table[i]=listeFree(ht.table[i]);
    }
  }




// ********************************************************************************************
// Partie 3
// ********************************************************************************************



//
// =========== A ECRIRE ================
// listeElemTable : retourne la liste des elements d'une table de hachage
// Entree : une HASTABLE ht
// Sortie : la liste de tous les couples (cle, valeur) presents dans la table
// Principe : il sagit de concatener toutes les listes de la table en une seule liste.

LISTE listeElemTable(HASHTABLE ht){
    int i;
    LISTE resultat=listeVide();
    for(i=0;i<ht.taille;i++){
        resultat=concat(resultat,ht.table[i]);
    }
    return resultat;
}




// =========== A ECRIRE ================
// tableListeElem : effectue le hachage d'une liste d'elements (listes de couples (cle,valeur))
//                  dans une table de hachage de taille t
// Entree : la LISTE l des ELEMENTs
//          la taille t de la table de hachage a creer
// Sortie : une table de hachage dans laquelle la LISTE l a ete hachee
// Attention : il ne s'agit pas d'inserer les cles dans la table avec l'operation hashTableInserer
//             mais d'initialiser une table et de la remplir avec les ELEMENTs de la liste l en ajoutant
//             chaque ELEMENT (cle, valeur) de l dans la liste situee, dans la table, a l'indice de hachage de la cle

HASHTABLE tableListeElem(LISTE l, int t){
  HASHTABLE ht=hashTableInit(t);
  LISTE p=l;
  while (p!=NULL) {
    ht.table[hash(p->elem.cle,t)]=listeAjouter(p->elem,ht.table[hash(p->elem.cle,t)]);
    p=p->succ;
  }
  return ht;
}




// =========== A ECRIRE ================
// listeVocabulaire : Supprime de la liste les mots d'occurrence < mini et > a maxi
// Entree : une liste l d'ELEMENTs
// Sortie : la listes des ELEMMENTs de l dont la valeur est superieure ou egale a mini
//          et inferieure ou egale a maxi

LISTE listeVocabulaire(LISTE l, int mini, int maxi){

     LISTE po = listeVide();
     LISTE p=l;
     while(p!=NULL){
       if(!(p->elem.valeur<mini || p->elem.valeur>maxi)){
         po=listeAjouter(p->elem,po);
            }
            p=p->succ;
          }
        return po;
      }




// =========== A ECRIRE ================
// compteCles : compte le nombre de cles de la liste d'ELEMENTs l qui sont dans la HASTABLE ht
// Entree : une LISTE l de d'ELEMENTs (cle, valeur) et une HASTABLE ht
// Sortie : le nombre de cles de l trouvees dans la HASTABLE h



  int compteCles(LISTE l, HASHTABLE ht){
    int compteur=0;
    while(l->succ!=NULL){
        char* cle = l->elem.cle;
        if(hashTableChercher(ht,cle)!=NULL) {compteur++;};

        l=l->succ;
    }
    return compteur;
  }





// =========== A ECRIRE ================
// nbClesCommunes : compte le nombre de clés communes a deux listes d'ELEMENTs l1 et l2
// Entree : deux lsites d'ELEMENTs l1 et l2
// Sortie : le nombre de clés communes
// Principe : - hacher la liste l1 dans une HASTABLE ht de taille par defaut TAILLEDEFAUT
//            - compter le nombre de cles de l2 qui sont dans la table ht

int nbClesCommunes(LISTE l1, LISTE l2){
  HASHTABLE ht;
  ht=tableListeElem(l1,TAILLEDEFAUT);
  return compteCles(l2,ht);
}






// Lecture d'un fichier mot a mot est stockage dans une table
// Entree : une HASTABLE ht et un pointeur de fichier filein
// Sortie : rien
// Effet de bord : tous les mots du fichier de texte designe par filein
//                 sont inserer dans la HASTABLE ht.
//                 En sortant ht contient donc tous les mots du fichier et leur nombre d'occurrence

void hashFichier(HASHTABLE ht, FILE* filein)
{
    char mot[30];
    while (!feof(filein)){

        fscanf(filein, "%s",mot);
        hashTableInserer(ht,mot);
    }
}




// ********************************************************************************************
// Programme principal
// ********************************************************************************************

int main()
{
    HASHTABLE ht1, ht2;
    char nomfichier1[30]="";
    char nomfichier2[30]="";
    FILE* fich1, *fich2;

    printf("\n\n***************************************************************************************");
    printf("\n*** Traitement du premier fichier \n");
    printf("\nDonner le nom du premier fichier: ");
    scanf("%s", nomfichier1);

    // lecture et hachage du fichier 1
    // Initialisation de la table ht1
    ht1 = hashTableInit(TAILLEDEFAUT);
    if ((fich1 = fopen(nomfichier1, "r")) == 0) {
        fprintf(stderr, "Probleme d'ouverture : %s\n", nomfichier1);
        exit(1);
    }
    hashFichier(ht1, fich1);
    fclose(fich1);

    // Affichage du contenu de la table de hachage du fichier1
    printf("\n\n*** Contenu de la table de hachage obtenue a partir du fichier : %s\n", nomfichier1);

    // Decommenter pour afficher le contenu de la table de hachage
    hashTableAfficher(ht1);

    // Extraction du vocabulaire du fichier1
    LISTE l1 = listeElemTable(ht1);
    // On ne conserve que les mots présents au moins 10 fois et au plus 100 fois
    LISTE lvoc1 = listeVocabulaire(l1, 10, 100);
    printf("\n\n*** Liste du vocabualire de %s: \n", nomfichier1);
    listeAfficher(lvoc1);




    printf("\n\n***************************************************************************************");
    printf("\n*** Traitement du deuxieme fichier \n");

    printf("\nDonner le nom du deuxième fichier: ");
    scanf("%s", nomfichier2);

    // lecture et hachage du fichier 2
    // Initialisation de la table ht2
    ht2 = hashTableInit(TAILLEDEFAUT);
    if ((fich2 = fopen(nomfichier2, "r")) == 0) {
        fprintf(stderr, "Probleme d'ouverture : %s\n", nomfichier2);
        exit(1);
    }
    hashFichier(ht2, fich2);
    fclose(fich2);

    // Affichage du contenu de la table de hachage du fichier2
    printf("\n\n*** Contenu de la table de hachage obtenue a partir du fichier : %s\n", nomfichier2);
    printf("\n(Decommenter la ligne correspondante dans le code source pour afficher le contenu de la table)");
    // Decommenter pour afficher le contenu de la table de hachage
    hashTableAfficher(ht2);

    // Extraction du vocabulaire du fichier2
    LISTE l2 = listeElemTable(ht2);
    // On ne conserve que les mots présents au moins 10 fois et au plus 100 fois
    LISTE lvoc2 = listeVocabulaire(l2, 10, 100);
    printf("\n\n*** Liste du vocabualire de %s: \n", nomfichier2);
    listeAfficher(lvoc2);


    // Calcul du nombre de mots de vocabulaire communs a fichier1 et fichier2
    // = cardinal de l'intersection des deux ensembles de vocabulaire
    int nbMotsCommun = nbClesCommunes(lvoc1, lvoc2);

    // Calcul du cardinal de l'union des deux ensembles de vovabulaire
    int lg1 = listeLongueur(lvoc1);  // Nombre de mots de lvoc1
    int lg2 = listeLongueur(lvoc2);  // Nombre de mots de lvoc2
    int lgunion = lg1 + lg2 - nbMotsCommun;

    printf("\n\n***************************************************************************************");
    printf("\n*** Resultats : \n");

    printf("\n\nNombre de mots du vocabulaire de %s : %d mots", nomfichier1, lg1);
    printf("\nNombre de mots du vocabulaire de %s : %d mots", nomfichier2, lg2);
    printf("\nNombre de mots dans la reunion des deux vocabulaire : %d mots", lgunion);
    printf("\nNombre de mots communs aux deux vocabulaires : %d", nbMotsCommun);

    printf("\n\nSimilarité des deux textes : %.2f\n", (100.0 * nbMotsCommun) / lgunion);

    return 0;


}



