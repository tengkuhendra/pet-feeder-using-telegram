//TELEGRAM
#include<KRtelegram.h>
char* ssid      = "XL Home_A13E";
char* pass      = "enakandirumah0200";
char* token     = "5127183849:AAH03ZfVFKVhwzyoWjK-RoLGAmMudNenoMU";
uint64_t chatid = 744127788;
int noJadwal = 0;
ReplyKeyboard telegramTombol;

//BUZZER
#define buzzer D8

//SERVO
#include<Servo.h>
Servo mekanik;
#define servoPin D4

//RTC
#include<KRrtc.h>

//LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//JADWAL
int jam1 = 7;
int menit1 = 15;
int jam2 = 11;
int menit2 = 45;
int jam3 = 17;
int menit3 = 55;

//#EEPROM
#include<EEPROM.h>


void setup() {
  Serial.begin(115200);

  //#EEPROM
  EEPROM.begin(512);
  jam1 = EEPROM.read(1);
  menit1 = EEPROM.read(2);
  jam2 = EEPROM.read(3);
  menit2 = EEPROM.read(4);
  jam3 = EEPROM.read(5);
  menit3 = EEPROM.read(6);

  //LCD
  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  //RTC
  rtcBegin();
  //setComputer();

  //BUZZER
  pinMode(buzzer, OUTPUT);

  //TELEGRAM
  setTelegram(ssid, pass, token);
  telegramTombol.addButton("/kasihPakan");
  telegramTombol.addRow();
  telegramTombol.addButton("/cekJadwal");
  telegramTombol.addButton("/aturJadwal");
  telegramTombol.enableResize();

  buzz(3, 150);
}

void loop() {

  //TELEGRAM
  TBMessage msg;
  if (myBot.getNewMessage(msg)) {
    String pesan = msg.text;
    Serial.println(pesan);

    if (noJadwal > 0) {
      if (noJadwal == 1) {
        String nilai = pesan;
        String nilai0 = getValue(nilai, ':', 0);
        String nilai1 = getValue(nilai, ':', 1);
        jam1 = nilai0.toInt();
        menit1 = nilai1.toInt();
        EEPROM.write(1, jam1);
        EEPROM.write(2, menit1);
        EEPROM.commit();
        String isiPesan = "Jadwal1 telah diubah menjadi " + nilai0 + ":" + nilai1;
        myBot.sendMessage(msg, isiPesan);
      }
      if (noJadwal == 2) {
        String nilai = pesan;
        String nilai0 = getValue(nilai, ':', 0);
        String nilai1 = getValue(nilai, ':', 1);
        jam2 = nilai0.toInt();
        menit2 = nilai1.toInt();
        EEPROM.write(3, jam2);
        EEPROM.write(4, menit2);
        EEPROM.commit();
        String isiPesan = "Jadwal2 telah diubah menjadi " + nilai0 + ":" + nilai1;
        myBot.sendMessage(msg, isiPesan);
      }
      if (noJadwal == 3) {
        String nilai = pesan;
        String nilai0 = getValue(nilai, ':', 0);
        String nilai1 = getValue(nilai, ':', 1);
        jam3 = nilai0.toInt();
        menit3 = nilai1.toInt();
        EEPROM.write(5, jam3);
        EEPROM.write(6, menit3);
        EEPROM.commit();
        String isiPesan = "Jadwal3 telah diubah menjadi " + nilai0 + ":" + nilai1;
        myBot.sendMessage(msg, isiPesan);
      }
      noJadwal = 0;
    }


    if (pesan == "/start") {
      String isiPesan = "Selamat datang di Pakan Ikan Otomatis Bot, silakan gunakan perintah ini\n\n";
      isiPesan += "/kasihPakan untuk memberikan pakan\n";
      isiPesan += "/cekJadwal untuk lihat daftar jadwal\n";
      isiPesan += "/aturJadwal untuk mengatur jadwal\n";
      isiPesan += "/bukaTombol untuk bukaTombol\n";
      myBot.sendMessage(msg, isiPesan);
    }

    if (pesan == "/bukaTombol") {
      myBot.sendMessage(msg, "Tombol Berhasil Dibuka:", telegramTombol);
    }


    if (pesan == "/kasihPakan") {
      buzz(2, 150);
      kasihPakan(15);
      myBot.sendTo(chatid, "Pakan Berhasil Diberikan!");
    }

    if (pesan == "/cekJadwal") {
      String isiPesan = "Daftar Jadwal\n\n";
      isiPesan += String() + "Jadwal1: " + jam1 + ":" + menit1 + "\n";
      isiPesan += String() + "Jadwal2: " + jam2 + ":" + menit2 + "\n";
      isiPesan += String() + "Jadwal3: " + jam3 + ":" + menit3 + "\n";
      myBot.sendMessage(msg, isiPesan);
    }

    if (pesan == "/aturJadwal") {
      String isiPesan = "Silakan pilih jadwal berapa yang mau diatur:\n\n";
      isiPesan += "/jadwal1 untuk memilih jadwal 1\n";
      isiPesan += "/jadwal2 untuk memilih jadwal 2\n";
      isiPesan += "/jadwal3 untuk memilih jadwal 3\n";
      myBot.sendMessage(msg, isiPesan);
    }

    if (pesan == "/jadwal1") {
      String isiPesan = "Silakan masukan waktu untuk jadwal1 dengan format jam:menit, contoh 4:35";
      myBot.sendMessage(msg, isiPesan);
      noJadwal = 1;
    }

    if (pesan == "/jadwal2") {
      String isiPesan = "Silakan masukan waktu untuk jadwal2 dengan format jam:menit, contoh 4:35";
      myBot.sendMessage(msg, isiPesan);
      noJadwal = 2;
    }

    if (pesan == "/jadwal3") {
      String isiPesan = "Silakan masukan waktu untuk jadwal3 dengan format jam:menit, contoh 4:35";
      myBot.sendMessage(msg, isiPesan);
      noJadwal = 3;
    }




  }

  //RTC
  rtcGet();
  Serial.println(String() + tanggal + "-" + bulan + "-" + tahun);
  Serial.println(String() + jam + ":" + menit + ":" + detik);

  //LCD
  lcd.setCursor(0, 0);
  lcd.print("Pakan Ikan Bot");
  lcd.setCursor(0, 1);
  lcd.print(String() + jam + ":" + menit + ":" + detik + "   ");

  //JADWAL
  if ((jam == jam1) && (menit == menit1) && (detik >= 0) && (detik <= 3)) {
    buzz(2, 150);
    kasihPakan(15);
    myBot.sendTo(chatid, "Pakan Jadwal1 Berhasil Diberikan!");
  }
  if ((jam == jam2) && (menit == menit2) && (detik >= 0) && (detik <= 3)) {
    buzz(2, 150);
    kasihPakan(15);
    myBot.sendTo(chatid, "Pakan Jadwal2 Berhasil Diberikan!");
  }
  if ((jam == jam3) && (menit == menit3) && (detik >= 0) && (detik <= 3)) {
    buzz(2, 150);
    kasihPakan(15);
    myBot.sendTo(chatid, "Pakan Jadwal3 Berhasil Diberikan!");
  }

}

//FUNGSI SPLIT STRING
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

//FUNGSI SERVO
void kasihPakan(int jumlah) {
  mekanik.attach(servoPin);
  for (int i = 0; i < jumlah; i++) {
    mekanik.write(150);
    delay(250);
    mekanik.write(0);
    delay(250);
  }
  mekanik.detach();
}
//FUNGSI BUZZER
void buzz(int jumlah, int Delay) {
  for (int i = 0; i < jumlah; i++) {
    digitalWrite(buzzer, HIGH);
    delay(Delay);
    digitalWrite(buzzer, LOW);
    delay(Delay);
  }
}
