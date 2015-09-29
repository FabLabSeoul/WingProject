
#include <windows.h>
#include <iostream>
#include "BufferedSerial.h"
#include "protocol.h"

using namespace std;

void SendCommand(CBufferedSerial &serial, const unsigned char cmd);
void RecvCommand(CBufferedSerial &serial);

void main(char argc, char *argv[])
{
	if (argc < 3)
	{
		cout << "commandline <PortNum> <BaudRate>" << endl;
		return;
	}
	
	const int portNum = atoi(argv[1]);
	const int baudRate = atoi(argv[2]);

	CBufferedSerial serial;
	serial.SetMaxWaitTime(1);
	if (!serial.Open(portNum, baudRate))
	{
		cout << "invalid port number" << endl;
		return;
	}

	cout << "Success Serial Connect" << endl;

	bool loop = true;
	while (loop)
	{
		if (!serial.IsOpened())
		{
			loop = false;
			break;
		}

		cout << "input command" << endl;
		char buff[64];
		cin >> buff;
		const string cmd = buff;

		if (cmd== "mag")
		{
			cout << "send mag calibration..." << endl;
			SendCommand(serial, MSP_MAG_CALIBRATION);
		}
		else if (cmd == "acc")
		{
			cout << "send accel calibration..." << endl;
			SendCommand(serial, MSP_ACC_CALIBRATION);
		}
		else if (cmd == "raw")
		{
			cout << "send raw imu data..." << endl;
			SendCommand(serial, MSP_RAW_IMU);
			RecvCommand(serial);
		}
		else if (cmd == "attitude_loop")
		{
			cout << "send attitude loop  data..." << endl;
			while (1)
			{
				SendCommand(serial, MSP_ATTITUDE);
				RecvCommand(serial);
				Sleep(10);
			}
		}
		else if (cmd == "exit")
		{
			cout << "close" << endl;
			loop = false;
		}
		else if (cmd == "#")
		{
			serial.SendData("#", 1);
		}
	}

	cout << "End" << endl;
}


void SendCommand(CBufferedSerial &serial, const unsigned char cmd)
{
	unsigned char packet[64];
	int checksum = 0;
	int idx = 0;
	packet[idx++] = '$';
	packet[idx++] = 'M';
	packet[idx++] = '<';
	packet[idx++] = 0;
	checksum ^= 0;
	packet[idx++] = cmd;
	checksum ^= cmd;
	packet[idx++] = checksum;
	serial.SendData((char*)packet, idx);
}


void RecvCommand(CBufferedSerial &serial)
{
	int state = 0;
	int len = 0;
	int readLen = 0;
	int msp = 0;
	int noDataCnt = 0;
	int checkSum = 0;
	while (1)
	{
		unsigned char c;
		if (serial.ReadData(&c, 1) <= 0)
		{
			Sleep(1);
			++noDataCnt;
			if (noDataCnt > 500)
				break; // exception
			continue;
		}

		switch (state)
		{
		case 0:
		{
			state = (c == '$') ? 1 : 0;
			cout << c;
		}
		break;
		
		case 1:
		{
			state = (c == 'M') ? 2 : 0;
			cout << c;
		}
		break;

		case 2:
		{
			state = (c == '>') ? 3 : 0;
			cout << c;
		}
		break;

		case 3:
		{
			len = c;
			cout << (int)c;
			checkSum ^= c;
			state = 4;
		}
		break;

		case 4:
		{
			msp = c;
			cout << (int)c << " ";
			checkSum ^= c;
			state = 5;
		}
		break;

		case 5:
		{
			if (len > readLen)
			{
				checkSum ^= c;
				cout << (int)c << " ";
			}
			else
			{
				if (checkSum == c)
				{
					cout << "ok" << endl;
					return; // end;
				}
				else
				{
					cout << "error!!" << endl;
					return; // end;
				}
			}

			++readLen;
		}
		break;

		default:
			break;
		}
	}
}
