/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package beans;

/**
 *
 * @author Vladimir
 */
public class Airport {
    String iata="";
    String naziv="";
    String city="";
    String country="";
    int br_pisti=0;
    int br_terminala=0;
    float latitude=0;
    float longitude=0;

    public Airport() {
    }


    public String getIata() {
        return iata;
    }

    public void setIata(String iata) {
        this.iata = iata;
    }

    public String getNaziv() {
        return naziv;
    }

    public void setNaziv(String naziv) {
        this.naziv = naziv;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public String getCountry() {
        return country;
    }

    public void setCountry(String country) {
        this.country = country;
    }

    public int getBr_pisti() {
        return br_pisti;
    }

    public void setBr_pisti(int br_pisti) {
        this.br_pisti = br_pisti;
    }

    public int getBr_terminala() {
        return br_terminala;
    }

    public void setBr_terminala(int br_terminala) {
        this.br_terminala = br_terminala;
    }

    public float getLatitude() {
        return latitude;
    }

    public void setLatitude(float latitude) {
        this.latitude = latitude;
    }

    public float getLongitude() {
        return longitude;
    }

    public void setLongitude(float longitude) {
        this.longitude = longitude;
    }
    
    
    
    
}
