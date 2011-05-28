#include <unistd.h>
#include <SerialStream.h>
#include <SerialPort.h>
#include <stdlib.h>
#include <stdio.h>

SerialPort *odo_serial;

double odo_x = 0.0;
double odo_y = 0.0;
double odo_a = 0.0;
double odo_g = 0.0;
double odo_d = 0.0;

void odo_flush()
{
    while (odo_serial->IsDataAvailable()) {
	odo_serial->ReadByte();
    }
}

#define RBUF_SIZE 1000
char rbuf[RBUF_SIZE];
int rb_cur=0;

bool odo_init()
{
    int i;
    bool ok = false;
    
    
    char port[1000] ="";
    //port = (char*)"";

    for (i = 0; i < 255; ++i) {
	sprintf(port, "/dev/ttyUSB%d", i);
	odo_serial = new SerialPort(port);
	odo_serial->SetBaudRate(SerialPort::BAUD_115200);
	odo_serial->Open();

	if (odo_serial->IsOpen()) {
	    odo_flush();
	    odo_serial->Write("?#");
	    usleep(50000);


	    while (odo_serial->IsDataAvailable()) {
		char c = odo_serial->ReadByte();
		if (c == 0xA)
		    continue;
		rbuf[rb_cur++] = c;
		rb_cur %= RBUF_SIZE - 2;
		if (c == '#') {
		    rbuf[rb_cur++] = '\0';
		    rb_cur = 0;
		    printf("recv %s\n", rbuf);

		    char c0 = rbuf[0];

		    if (c0 == '?') {
			if (rbuf[1] != 'o') {
			    break;
			} else {
			    odo_serial->Close();
			    delete odo_serial;
			    continue;
			}
		    } else {
			odo_serial->Close();
			delete odo_serial;
			continue;
		    }
		}
	    }
	}

	ok = true;
    }


    return ok;
}

#define PI 3.14159265359

void odo_get_coord()
{
    odo_serial->Write("R#");
    odo_serial->Write("r#");

    usleep(50000);

    while (odo_serial->IsDataAvailable()) {
	char c = odo_serial->ReadByte();
	if (c == 0xA)
	    continue;
	rbuf[rb_cur++] = c;
	rb_cur %= RBUF_SIZE - 2;
	if (c == '#') {
	    rbuf[rb_cur++] = '\0';
	    rb_cur = 0;
	    printf("recv %s\n", rbuf);

	    char c0 = rbuf[0];

	    if (c0 == 'X') {
		long nx;
		sscanf(rbuf, "X%ld#", &nx);
		odo_x = (double) nx / 1000.0;
	    }
	    if (c0 == 'Y') {
		long ny;
		sscanf(rbuf, "Y%ld#", &ny);
		odo_y = (double) ny / 1000.0;
	    }
	    if (c0 == 'A') {
		long na;
		sscanf(rbuf, "A%ld#", &na);
		odo_a = PI + (double) na / 1000.0;
	    }
	    if (c0 == 'G') {
		double ng;
		sscanf(rbuf, "G%lf#", &ng);
		odo_g = ng;
	    }
	    if (c0 == 'D') {
		double nd;
		sscanf(rbuf, "D%lf#", &nd);
		odo_d = nd;
	    }
	}
	if (!(odo_serial->IsDataAvailable()))
	    sleep(50000);
    }
}
