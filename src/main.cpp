/*******************************************************************
 * Projeto: POINTS
 * Criador: Gustavo Souza (Berlandev)
 * Data: 11/01/2022
 * 
 *******************************************************************/

#include "painlessMesh.h"
#include "mesh.cpp"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub




void sendMessage() ; // Prototype so PlatformIO doesn't complain
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "(B) Mensagem do B ";
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("(B) Origem: Ponto B (iD: %u) msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("-->(B) startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("(B) Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf(" (B) Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}


void setup() {
/*******************************************************************
 * Iniciando Comunicação Serial
 *******************************************************************/
  Serial.begin(9600);
  Serial.println("-----------------------------------------------------------------------------------------------------------------");
  Serial.println("Lista de Serviços Iniciados:");
  Serial.println("Comunicação Serial");
/*******************************************************************
 * Iniciando Spiffs (Envio de arquivo)
 *******************************************************************/
  if(!SPIFFS.begin(true)){
    Serial.println("Ocorreu um erro iniciando o SPIFFS");
    return;
  }
  Serial.println("-----------------------------------------------------------------------------------------------------------------");
  Serial.println("SPIFFS");
  Serial.println("Arquivos:");
 // Listar arquivos
  File root = SPIFFS.open("/");
   File file = root.openNextFile();
 
  while(file){
      Serial.print("--FILE: ");
      Serial.println(file.name());
 
      file = root.openNextFile();
  }
      Serial.println("Nenhum arquivo encontrado");
  /*******************************************************************
 * Iniciando Mesh
 *******************************************************************/
  Serial.println("-----------------------------------------------------------------------------------------------------------------");
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
  Serial.println("-----------------------------------------------------------------------------------------------------------------");
  Serial.println("Mesh");

}

void loop() {
  mesh.update();
  delay(1000);
}