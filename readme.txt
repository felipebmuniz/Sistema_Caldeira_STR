Compilar e Executar o arquivo main.c na pasta functions com o comando e cancelar apostos a coleta de dados:

gcc -o main main.c functions/tela/info_tela.c functions/monitores/monitor_sensor.c functions/monitores/monitor_tela.c functions/buffers/buffer_duplo_sensores.c functions/buffers/buffer_duplo_resposta.c functions/socket/socket.c -lpthread -lr
./main localhost 4545