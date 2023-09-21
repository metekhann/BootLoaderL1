#include <stdio.h>
#include <windows.h>
#include <string.h>
//#include <stddef.h>
#include <stdint.h>
#include <conio.h>

#define BUFFER_SIZE 32


int init_com_port(HANDLE* port, char* portname);
long int listen_info(HANDLE* port, long int* file_size);
int send_tamam(HANDLE* port);
int listen_data(HANDLE* port, char* buffer, long int file_size);
int send_data_ok(HANDLE* port);
int listen_crc(HANDLE* port, uint32_t* crc);
int write_file(char*buffer, char* fileName , long int file_size);
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
int send_info(HANDLE* port,char* fileName, long int* file_size);
int tamam_onay(HANDLE* port);
int send_name(HANDLE* port, char* fileName);
int send_data(HANDLE* port,char* fileName, long int file_size, uint32_t* crc);
int data_ok_onay(HANDLE* port);
int send_crc(HANDLE* port, uint32_t crc);

uint32_t crc32(const char *s,size_t n);



void command_selector(HANDLE* port, char* portname, uint8_t command);
void send_get_ver(HANDLE* port, char* portname);
void listen_ack(HANDLE* port, char* portname);

int main() {
    HANDLE hSerial = {0};
    char portname[10];
    FILE* file;
    char fileName[32];
    char* buffer;  
    DWORD bytesRead = 0;
    DWORD bytesWritten = 0;
    long int file_size = 0;
    uint32_t crc = 0;
    uint32_t calculated_crc = 0;

    uint8_t command_select = 0;

    printf("Port Name Girin:\n");
    scanf("%s",portname);



    // Seri portu init etme

    if(init_com_port(&hSerial, portname))
    {
        printf("basarili_init\n");
    }
    else
    {
        printf("basarisiz_init\n");
        return 0;
    }
    printf("Datalar bekleniyor....\n");



    while(1)
    {
        printf("BL_GET_VER------------------->>1\n");
        printf("BL_GET_HELP------------------>>2\n");
        printf("BL_GET_CID------------------->>3\n");
        printf("BL_GET_RDP_STATUS------------>>4\n");
        printf("BL_GO_TO_ADRR---------------->>5\n");
        printf("BL_FLASH_ERASE--------------->>6\n");
        printf("BL_MEM_WRITE----------------->>7\n");
        printf("BL_EN_RW_PROTECT------------->>8\n");
        printf("BL_MEM_READ------------------>>9\n");
        printf("BL_READ_SECTOR_P_STATUS----->>10\n");
        printf("BL_OTP_READ----------------->>11\n");
        printf("BL_DIS_RW_PROTECT----------->>12\n");

        scanf("%u",&command_select);

        command_selector(&hSerial, portname, command_select);
        


    }
    



    CloseHandle(hSerial);

    printf("Program sonlandirildi. Dinleme durduruldu.\n");
    return 0;
}

void command_selector(HANDLE* port, char* portname, uint8_t command)
{
    switch(command)
    {
        case 1:

        send_get_ver(port, portname);
        listen_ack(port, portname);
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break; 
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        case 11:
            break; 
        case 12:
            break;                   
    }
}

void send_get_ver(HANDLE* port, char* portname)
{
    /******LENGHT*/
    DWORD bytesWritten;
    uint8_t follow_lenght = 5;
    uint8_t command = 81;
    uint32_t calculated_crc = 0;

    uint8_t send_ver_packet[2] = {0};
    send_ver_packet[0] = follow_lenght;
    send_ver_packet[1] = command;

    calculated_crc = crc32(send_ver_packet, 2);

    if(WriteFile(*port, send_ver_packet , sizeof(send_ver_packet), &bytesWritten, NULL))
    {
        printf("lenght yollandi..\n");
    }
    else
    {
        printf("hata: lenght gonderilemedi..");
    }

    if(WriteFile(*port, &calculated_crc , sizeof(calculated_crc), &bytesWritten, NULL))
    {
        printf("crc yollandi..\n");
    }
    else
    {
        printf("hata: veri gonderilemedi..");
    }
}
void listen_ack(HANDLE* port, char* portname)
{
    int gecikme = 1;
    DWORD bytesRead = 0;
    uint8_t size = 2;
    uint8_t temp_buffer[2] = {0};
 
    while((size > 0) && (gecikme < 40))
    {
        if (ReadFile(*port, temp_buffer, 2, &bytesRead, NULL)) 
        {
            printf("%lu\n",bytesRead);
            if (bytesRead > 0) 
            {
                //memcpy(buffer, temp_buffer, bytesRead);
                //buffer = buffer + bytesRead; 
                size = size - bytesRead;
            }
        } 
        else 
        {
            printf("Hata: Veri okunamadi\n");
            break;
        }
        if(bytesRead == 0)
            gecikme++;
    }
    if(temp_buffer[0] == 0xA5U)
    {
        printf("VERSION\n: %u", temp_buffer[1]);

    }
    else
    {
        printf("NACK\n");
    }
}
    //if(gecikme >= 50)























int init_com_port(HANDLE* port, char* portname)
{
////// Seri Portu açma
    *port = CreateFile(portname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (*port == INVALID_HANDLE_VALUE) 
    {
        printf("Hata: Seri port acilamadi\n");
        return 0;
    }

///////// Seri port ayarlarini yapma
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(*port, &dcbSerialParams)) 
    {
        printf("Hata: Seri port ayarlari alinamadi\n");
        CloseHandle(*port);
        return 0;
    }
    dcbSerialParams.BaudRate = CBR_115200;  // İletim hızını belirle (örneğin 9600)
    dcbSerialParams.ByteSize = 8;         // Veri boyutunu belirle (8 bit)
    dcbSerialParams.Parity = NOPARITY;    // Pariteyi belirle (parite yok)
    dcbSerialParams.StopBits = ONESTOPBIT;// Durma bitlerini belirle (1 bit)
    if (!SetCommState(*port, &dcbSerialParams)) 
    {
        printf("Seri port ayarlari yapilamadi\n");
        CloseHandle(*port);
        return 0;
    }

/////// Timeouts ayarlarini yapma (bloklayici olmaktan cikarma)
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutMultiplier = 200;
    timeouts.ReadTotalTimeoutConstant = 800;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    SetCommTimeouts(*port, &timeouts);
    return 1;
}

long int listen_info(HANDLE* port, long int* file_size)
{

    DWORD bytesRead = 0;
    char ch;

    while(1)
    {
        printf("..\n");
        if (ReadFile(*port, file_size, sizeof(long int), &bytesRead, NULL)) 
        {
            //printf("%d",buffer);
            if (bytesRead > 0) 
            {       
       
                return 1;
            }
        } 
        else 
        {
            printf("Hata: Veri okunamadi\n");
            break;
        }
        if (_kbhit()) 
        {
            ch = _getch();
            if (ch == 'q') 
            {
                printf("Programdan çıkılıyor...\n");
                return 0;
         
            }   
        }
        
    }
    
}

int send_tamam(HANDLE* port)
{
    DWORD bytesWritten;
    if(WriteFile(*port, "tamam", 6*sizeof(char), &bytesWritten, NULL))
    {
        printf("tamam yollandi..\n");
    }
    else
    {
        printf("hata: veri gonderilemedi..");
    }
}
int send_data_ok(HANDLE* port)
{
    char data_tamam[]= "data_tamam";
    DWORD bytesWritten;
    if(WriteFile(*port, data_tamam , sizeof(data_tamam), &bytesWritten, NULL))
    {
        printf("data_tamam yollandi..\n");
    }
    else
    {
        printf("hata: veri gonderilemedi..");
    }
}

int listen_data(HANDLE* port, char* buffer, long int file_size)
{
    int gecikme = 1;
    DWORD bytesRead = 0;
    char temp_buffer[BUFFER_SIZE] = {0};
 
    while((file_size > 0) && (gecikme < 40))
    {
        if (ReadFile(*port, temp_buffer, BUFFER_SIZE, &bytesRead, NULL)) 
        {
            printf("%lu\n",bytesRead);
            if (bytesRead > 0) 
            {
                memcpy(buffer, temp_buffer, bytesRead);
                buffer = buffer + bytesRead; 
                file_size = file_size - bytesRead;
            }
        } 
        else 
        {
            printf("Hata: Veri okunamadi\n");
            break;
        }
        if(bytesRead == 0)
            gecikme++;
    }

    if(gecikme >= 50)
        return 0;
    
    return 1;

}
int listen_crc(HANDLE* port, uint32_t* crc)
{
    DWORD bytesRead = 0;
    for (int i = 0; i < 10; i++)
    {
        if(ReadFile(*port, &(*crc), sizeof(uint32_t), &bytesRead, NULL))
        {
            if(bytesRead > 0)
            {
                return 1;
                printf("listen_crc:%lu\n",*crc);
            }
        }
        else
        {
            printf("Hata: Veri okunamadi\n");
            return 0;
        }
    }
    return 0;
}
int listen_fileName(HANDLE* port, char*fileName)
{
    DWORD bytesRead = 0;
    for (int i = 0; i < 10; i++)
    {
        if(ReadFile(*port, fileName, 32 * sizeof(char), &bytesRead, NULL))
        {
            if(bytesRead > 0)
            {
                return 1;
                printf("FileName:%s\n",fileName);
            }
        }
        else
        {
            printf("Hata: Veri okunamadi\n");
            return 0;
        }
    }
    return 0;
}

int write_file(char*buffer, char* fileName , long int file_size)
{
    printf("FileName:%s\n",fileName);
    FILE* file = fopen(fileName, "wb");
    if (!file) 
    {
        printf("Dosya acilamadi\n");
        
        return 1;
    }
    fwrite(buffer, 1, file_size, file);
    fclose(file);
}

uint32_t crc32(const char *s,size_t n) 
{
	uint32_t crc=0xFFFFFFFF;
	
	for(size_t i=0;i<n;i++) {
		char ch=s[i];
		for(size_t j=0;j<8;j++) {
			uint32_t b=(ch^crc)&1;
			crc>>=1;
			if(b) crc=crc^0xEDB88320;
			ch>>=1;
		}
	}
	
	return ~crc;
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////

int send_info(HANDLE* port, char* fileName, long int* file_size)
{
    DWORD bytesWritten = 0;
        // Dosyayı açma
    FILE* file = fopen(fileName, "rb");
    if (!file) {
        printf("Dosya acilamadi\n");
        CloseHandle(*port);
        return 1;
    }

    //Boyutunu bulma
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    printf("Giden File Size :%ld\n", *file_size); 
    fclose(file);

 /////////// Boyutunu karşıya yollama///////////
    WriteFile(*port, file_size , sizeof(long int), &bytesWritten, NULL);
    printf("%ld\n",file_size);
    printf("%lu\n", bytesWritten);
    return *file_size;
}
int tamam_onay(HANDLE* port)
{
    char tamam[20] = {0};
    DWORD bytesRead = 0;

    for(int i = 0; i < 10; i++)
    {
        if(ReadFile(*port, tamam, sizeof(tamam),&bytesRead,NULL))
        {
            if (bytesRead > 0)
            {
                break;
            }
        }
        else
        {
            printf("Hata: Veri okunamadi\n");
            break;
        }
    }

    if(strcmp(tamam,"tamam") == 0)
    {
        printf("Tamam alindi\n");
        return 1;
    }
    else
    {
        printf("Tamam alinamadi\n");
        return 0;
    }
}
int send_name(HANDLE* port, char* fileName)
{
    
    DWORD bytesWritten = 0;
 /////////// ///////////
    WriteFile(*port, fileName, 32 * sizeof(char), &bytesWritten, NULL);
    printf("%s\n",fileName);
    printf("%lu\n", bytesWritten);
}

int send_data(HANDLE* port,char* fileName, long int file_size, uint32_t* crc)
{
    printf("sendmidata\n");
    DWORD bytesRead = 0;
    DWORD bytesWritten = 0;
    printf("%ld\n",file_size);
    char* buffer = (char *)malloc(2 * file_size * sizeof(char));
    //memory açıldı mı kontrolu
    if (buffer == NULL)
    {
        printf("Bellek tahsisi başarisiz.\n");
        return 0;
    }
    FILE* file = fopen(fileName, "rb");
    if (!file) {
        printf("Dosya acilamadi\n");
        CloseHandle(*port);
        return 0;
    }
    fseek(file,0,SEEK_SET);
    bytesRead = fread(buffer, 1, file_size + 1, file);
    *crc = crc32(buffer, bytesRead);
    printf("Dosyadan Okunan Data: %lu\n", bytesRead);
    WriteFile(*port, buffer,bytesRead, &bytesWritten, NULL);
    printf("COM Porta Yazilan Data: %lu\n",bytesWritten);
    free(buffer);
    fclose(file);
}

int send_crc(HANDLE* port, uint32_t crc)
{
    DWORD bytesWritten = 0;
    WriteFile(*port, &crc, sizeof(uint32_t), &bytesWritten, NULL);
    
}

int data_ok_onay(HANDLE* port)
{
    char data_tamam[20] = {0};
    DWORD bytesRead = 0;

    for(int i = 0; i < 10; i++)
    {
        if(ReadFile(*port, data_tamam, sizeof(data_tamam),&bytesRead,NULL))
        {
            if (bytesRead > 0)
            {
                break;
            }
        }
        else
        {
            printf("Hata: Veri okunamadi\n");
            break;
        }
    }

    if(strcmp(data_tamam,"data_tamam") == 0)
    {
        printf("data_tamam alindi\n");
        return 1;
    }
    else
    {
        printf("data_tamam_alinamadi\n");
        return 0;
    }
}