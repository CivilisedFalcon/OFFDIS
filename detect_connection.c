#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#define MAX_ARP 10
#define BUFFER_SIZE 255

typedef int IP_add[4];
typedef int MAC_add[6];
typedef struct {
	IP_add ip_add;
	MAC_add mac_add;
	char type;
}ARP;

IP_add HOTSPOT_IP = { 192, 168, 137, 1 };
ARP ARP_LIST[MAX_ARP];
FILE* users, * logs;

void DEVICE_CONNECT_INTERRUPT(ARP arp);
void DEVICE_DISCONNECT_INTERRUPT(ARP arp);
void refresh_arp();
void update_arp(ARP arp);
void make_arp(char* buf, ARP* arp);
int find_ip(char* buf, IP_add ip);
int find_mac(char* buf, MAC_add mac);
int find_type(char* buf, char *type);
void print_arp(ARP arp);
void log(int type, ARP arp);
void manage_user(int type, ARP arp);

int main()
{
	// First, set all objects in the ARP_LIST to null.
	for (int i = 0; i < MAX_ARP; i++)
		ARP_LIST[i].type = 'n';

	fopen_s(&log, "log.txt", "w+");
	fopen_s(&users, "users.txt", "w+");
	fclose(log);
	fclose(users);

	char buf[BUFFER_SIZE] = { 0, };
	ARP arp;
	IP_add ip;
	int c = 0;
	bool find = false;
	FILE* fp;
	
	while (1) 
	{
		fp = _popen("arp -a", "rt");
		while (fgets(buf, BUFFER_SIZE, fp))
		{
			if (strlen(buf) == 1)
			{
				fgets(buf, BUFFER_SIZE, fp);

				find_ip(buf, ip);
				if (memcmp(ip, HOTSPOT_IP, 4 * sizeof(int)) == 0) // When the interface IP address matches the hotspot address
				{
					//printf("Mobile Hotspot has been detected.\n");
					find = true;

					// Read one line at a time to make ARP.
					while (fgets(buf, BUFFER_SIZE, fp))
					{
						if (strlen(buf) == 1)
							break;
						make_arp(buf, &arp);
						update_arp(arp); //After make ARP, the change is updated in the ARP_LIST.
					}

					break;
				}
			}
		}
		_sleep(1000);

		refresh_arp();
		_pclose(fp);
	}


	return 0;
}

void DEVICE_CONNECT_INTERRUPT(ARP arp)
{
	manage_user(0, arp);
	log(0, arp);
}

void DEVICE_DISCONNECT_INTERRUPT(ARP arp)
{
	manage_user(1, arp);
	log(1, arp);
}

void refresh_arp()
{
	FILE* fp = NULL;
	char command[32], buf[BUFFER_SIZE];
	int i, j;

	// 연결끊긴 arp 감지
	for (i = 0; i < MAX_ARP; i++)
	{
		if (ARP_LIST[i].type == 'n'
			|| ARP_LIST[i].type == 'u')
			continue;

		sprintf_s(command, 32, "ping %d.%d.%d.%d -n 1", ARP_LIST[i].ip_add[0], ARP_LIST[i].ip_add[1], ARP_LIST[i].ip_add[2], ARP_LIST[i].ip_add[3]);
		fp = _popen(command, "rt");
		fgets(buf, BUFFER_SIZE, fp);
		fgets(buf, BUFFER_SIZE, fp);
		fgets(buf, BUFFER_SIZE, fp);
		if (!('0' < buf[0] && '9' > buf[0]))
		{
			ARP_LIST[i].type = 'u';
			DEVICE_DISCONNECT_INTERRUPT(ARP_LIST[i]);
		}
		_pclose(fp);
	}

	// 재연결된 arp 감지
	for (i = 0; i < MAX_ARP; i++)
	{
		if (ARP_LIST[i].type == 'u')
		{
			sprintf_s(command, 32, "ping %d.%d.%d.%d -n 1", ARP_LIST[i].ip_add[0], ARP_LIST[i].ip_add[1], ARP_LIST[i].ip_add[2], ARP_LIST[i].ip_add[3]);
			fp = _popen(command, "rt");
			fgets(buf, BUFFER_SIZE, fp);
			fgets(buf, BUFFER_SIZE, fp);
			fgets(buf, BUFFER_SIZE, fp);
			if ('0' < buf[0] && '9' > buf[0]) {
				ARP_LIST[i].type = 's';
				DEVICE_CONNECT_INTERRUPT(ARP_LIST[i]);
			}
			_pclose(fp);
		}
	}
}

void update_arp(ARP arp)
{
	if (arp.type != 'n') 
	{
		for (int i = 0; i < MAX_ARP; i++)
		{
			if (ARP_LIST[i].type == 'n') {
				// Make Interrupt when new ARP has been updated. (Temporarily using a function)
				ARP_LIST[i] = arp;
				DEVICE_CONNECT_INTERRUPT(arp);
				break;
			}
			if (memcmp(arp.mac_add, ARP_LIST[i].mac_add, sizeof(MAC_add)) == 0) {
				// Compare MAC addresses to check ARP already exists or not.
				break;
			}
		}
	}
}

void make_arp(char* buf, ARP* arp)
{
	int r1, r2, r3;
	
	r1 = find_ip(buf, arp->ip_add);
	r2 = find_mac(buf + r1, arp->mac_add);
	r3 = find_type(buf + r1 + r2, &arp->type);
}

int find_ip(char* buf, IP_add ip)
{
	int i = 0, j = 0;
	char c;
	for (i = 0; i < strlen(buf); i++)
	{
		c = buf[i];
		if (c > '0' && c < '9')
		{
			for (j = i; j < strlen(buf); j++)
			{
				c = buf[j];
				if (c == ' ')
					break;
			}
			
			break;
		}
	}

	char ipstring[20];
	strncpy_s(ipstring, 20, buf + i, j - i);
	sscanf_s(ipstring, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);


	return j;
}

int find_mac(char* buf, MAC_add mac)
{
	int i = 0, j = 0;
	char c;
	for (i = 0; i < strlen(buf); i++)
	{
		c = buf[i];
		if ((c >= '0' && c <= '9')
			|| (c >= 'a' && c <= 'f'))
		{
			for (j = i; j < strlen(buf); j++)
			{
				c = buf[j];
				if (c == ' ')
					break;
			}

			break;
		}
	}

	char macstring[20];
	strncpy_s(macstring, 20, buf + i, j - i);
	sscanf_s(macstring, "%x-%x-%x-%x-%x-%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
	

	return j;
}

int find_type(char* buf, char* type)
{
	int i = 0, j = 0;

	for (i = 0; i < strlen(buf); i++) 
	{
		if (buf[i] != ' ') 
		{
			for (j = i; j < strlen(buf); j++)
				if (buf[j] == ' ')
					break;
			break;
		}
	}
		
	char typestring[8];
	strncpy_s(typestring, 8, buf + i, j - i);
	if (strcmp(typestring, "정적") == 0)
		*type = 's';
	else if (strcmp(typestring, "동적") == 0)
		*type = 'd';
	else
		*type = 'n';


	return 0;
}

void print_arp(ARP arp)
{
	if (arp.type != 'n' && arp.type != 'u')
	{
		int i = 0;

		printf(" IP Address: ");
		for (i = 0; i < 3; i++)
			printf("%d.", arp.ip_add[i]);
		printf("%d\n", arp.ip_add[3]);

		printf(" MAC Address: ");
		for (i = 0; i < 5; i++)
			printf("%02X-", arp.mac_add[i]);
		printf("%02X\n", arp.mac_add[5]);

		printf(" Type: %c", arp.type);
		printf("\n");
	}
}

void log(int type, ARP arp)
{
	time_t current = time(NULL);
	struct tm t;
	localtime_s(&t, &current);
	char log[64];

	fopen_s(&logs, "log.txt", "a+");

	if (type == 0)
		sprintf_s(log, 64, "[%d-%d-%d %d:%d:%d] %02X:%02X:%02X:%02X:%02X:%02X has been connected.\n", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, arp.mac_add[0], arp.mac_add[1], arp.mac_add[2], arp.mac_add[3], arp.mac_add[4], arp.mac_add[5]);
	else if (type == 1)
		sprintf_s(log, 64, "[%d-%d-%d %d:%d:%d] %02X:%02X:%02X:%02X:%02X:%02X has been disconnected.\n", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, arp.mac_add[0], arp.mac_add[1], arp.mac_add[2], arp.mac_add[3], arp.mac_add[4], arp.mac_add[5]);
	else
		return;
	fwrite(log, sizeof(char), strlen(log), logs);
	printf("%s", log);

	fclose(logs);
}

void manage_user(int type, ARP arp)
{
	int i = 0;
	char user[64], _user[MAX_ARP][64];
	fopen_s(&users, "users.txt", "r+");

	sprintf_s(user, 64, "%d.%d.%d.%d / %02X:%02X:%02X:%02X:%02X:%02X\n", arp.ip_add[0], arp.ip_add[1], arp.ip_add[2], arp.ip_add[3], arp.mac_add[0], arp.mac_add[1], arp.mac_add[2], arp.mac_add[3], arp.mac_add[4], arp.mac_add[5]);

	if (type == 0)
	{
		while (fgets(_user[i], 64, users))
		{
			if (strcmp(user, _user[i]) == 0) {
				fclose(users);
				return;
			}
		}

		fwrite(user, sizeof(char), strlen(user), users);
		fclose(users);
	}
	else if (type == 1)
	{
		FILE* tmp;
		fopen_s(&tmp, "tmp.txt", "w");

		while (fgets(_user[i], 64, users))
		{
			if (strcmp(user, _user[i]) == 0)
				i--;

			i++;
		}

		for (int j = 0; j < i; j++)
			fwrite(_user[j], sizeof(char), strlen(_user[j]), tmp);

		fclose(users);
		fclose(tmp);
		remove("users.txt");
		rename("tmp.txt", "users.txt");
	}
	else
	{

	}

	//fclose(tmp);
	//fclose(users);
}