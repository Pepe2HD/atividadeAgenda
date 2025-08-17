#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char nome[128];
    char telefone[16];
} Contato;

typedef struct {
    Contato contatos[100];
    int contContato;
} Agenda;

int verifNome(char nome[]);
int verifTel(char telefone[]);
void inserirContato(Agenda *a);
void apagarContato(Agenda *a, int id);
void buscarContato(Agenda *a);
void listarContatos(Agenda *a);
void salvarAgenda(Agenda *a, const char *nomeArquivo);
void carregarAgenda(Agenda *a, const char *nomeArquivo);

int main() {
    Agenda agenda;
    agenda.contContato = 0;

    carregarAgenda(&agenda, "agenda.csv"); 

    int op = 1, opMenu, id;

    while(op != 0) {
        printf("\n--- MENU ---\n");
        printf("Bem vindo ao seu mais famoso app AddCont!\nQual vai ser sua opção?\n\n");
        printf("1 - Inserir contato\n");
        printf("2 - Apagar contato\n");
        printf("3 - Buscar contato\n");
        printf("4 - Listar contatos\n");
        printf("0 - Sair\n");
        printf("Digite: ");
        scanf("%d", &opMenu);

        switch(opMenu){
            case 1:
                inserirContato(&agenda);
                break;
            case 2:
                if(agenda.contContato == 0) {
                    printf("\nNenhum contato adicionado ainda!\n");
                    printf("Por favor, adicione um contato antes de remover.\n");
                    break;
                } else {
                    printf("Digite o ID do contato a ser removido: ");
                    scanf("%d", &id);
                    apagarContato(&agenda, id);
                }
                break;
            case 3:
                buscarContato(&agenda);
                break;
            case 4:
                listarContatos(&agenda);
                break;
            case 0:
                salvarAgenda(&agenda, "agenda.csv"); 
                op = 0;
                break;
            default:
                printf("Opção invalida!\n");
        }
    }

    return 0;
}

int verifNome(char nome[]) {
    int t = strlen(nome);
    if(t >= 128) {
        return 1; 
    } else {
        return 0; 
    }
}

int verifTel(char telefone[]) {
    int t = strlen(telefone);
    if(t > 16 || t < 13) {
        return 1;
    } else {
        return 0;
    }
}

void inserirContato(Agenda *a) {
    if(a->contContato >= 100) {
        printf("Infelizmente não pode adicionar mais contatos!\nRemova no minimo 1 para adicionar!\n");
        return;
    }

    Contato contato;
    contato.id = a->contContato + 1;

    int nomeVerif, telVerif;

    printf("\nDigite o nome do contato: ");
    scanf(" %s", contato.nome);

    nomeVerif = verifNome(contato.nome);
    while(nomeVerif == 1) {
        printf("Nome muito extenso! \nDigite novamente:");
        scanf(" %s", contato.nome);
        nomeVerif = verifNome(contato.nome);
    }

    printf("Digite o telefone do contato: ");
    scanf(" %s", contato.telefone);

    telVerif = verifTel(contato.telefone);
    while(telVerif == 1) {
        printf("\nNúmero de telefone no formato errado! \nDigite novamente:");
        scanf(" %s", contato.telefone);
        telVerif = verifTel(contato.telefone);
    }

    a->contatos[a->contContato++] = contato;

    printf("Contato adicionado! ID: %d\n", contato.id);
}

void apagarContato(Agenda *a, int id) {
    int i, encontrado = 0;
    for(i = 0; i < a->contContato; i++) {
        if(a->contatos[i].id == id) {
            encontrado = 1;
            
            for(int j = i; j < a->contContato - 1; j++) {
                a->contatos[j] = a->contatos[j+1];
                a->contatos[j].id = j + 1; 
            }
            a->contContato--;
            printf("Contato removido com sucesso!\n");
            break;
        }
    }
    if(!encontrado) {
        printf("Contato com ID %d nao encontrado.\n", id);
    }
}

void buscarContato(Agenda *a) {
    
    if(a->contContato == 0) {
        printf("\nNenhum contato adicionado ainda!\n");
        printf("Por favor, adicione um contato antes de buscar.\n");
        return;
    }

    char termo[128];
    int op, encontrado = 0;

    printf("\nOque você prefere?\nBuscar por: 1 - Nome | 2 - Telefone: ");
    scanf("%d", &op);

    if(op == 1) {
        printf("Digite o nome: ");
        scanf(" %127s", termo);

        for(int i = 0; i < a->contContato; i++) {
            if(strcmp(a->contatos[i].nome, termo) == 0) {
                printf("Encontrado -> ID:%d Nome:%s Telefone:%s\n", 
                        a->contatos[i].id, a->contatos[i].nome, a->contatos[i].telefone);
                encontrado = 1;
            }
        }
    } else if(op == 2) {
        printf("Digite o telefone: ");
        scanf(" %15s", termo);

        for(int i = 0; i < a->contContato; i++) {
            if(strcmp(a->contatos[i].telefone, termo) == 0) {
                printf("Encontrado -> ID:%d Nome:%s Telefone:%s\n", 
                        a->contatos[i].id, a->contatos[i].nome, a->contatos[i].telefone);
                encontrado = 1;
            }
        }
    }

    if(!encontrado) {
        printf("Contato nao encontrado.\n");
    }
}

void listarContatos(Agenda *a) {
    if(a->contContato == 0) {
        printf("Agenda vazia!\n");
        return;
    }
    printf("\n| ---        LISTA DE CONTATOS        --- |\n");
    for(int i = 0; i < a->contContato; i++) {
        printf("ID:%d Nome:%s Telefone:%s\n", 
                a->contatos[i].id, a->contatos[i].nome, a->contatos[i].telefone);
    }
}

void salvarAgenda(Agenda *a, const char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "w");
    if(f == NULL) {
        printf("Erro ao salvar a agenda!\n");
        return;
    }
    for(int i = 0; i < a->contContato; i++) {
        fprintf(f, "%d,%s,%s\n", a->contatos[i].id, a->contatos[i].nome, a->contatos[i].telefone);
    }
    fclose(f);
    printf("Agenda salva em %s\n", nomeArquivo);
}

void carregarAgenda(Agenda *a, const char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "r");
    if(f == NULL) {
        return; 
    }
    while(fscanf(f, "%d,%127[^,],%15s\n", &a->contatos[a->contContato].id, a->contatos[a->contContato].nome, a->contatos[a->contContato].telefone) == 3) {
        a->contContato++;
    }
    fclose(f);
}

