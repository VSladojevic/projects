/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package beans;

import com.sun.org.apache.xerces.internal.impl.dv.xs.DateTimeDV;
import java.sql.Time;
import java.sql.Timestamp;

/**
 *
 * @author Vladimir
 */
public class Flight {

    int id;
    String dep_airport; //iata
    Integer dep_gate; //gate_id
    String arr_airport; //iata
    Integer arr_gate; //gate_id
    Integer plane_id;
    int reserved_seats;
    Time duration; //java.sql.Time
    java.sql.Timestamp takeoff_time;
    java.sql.Timestamp planned_arrival;
    java.sql.Timestamp expected_arrival;
    java.sql.Timestamp arrival_time;
    Integer company; //company_id
    Integer pilot; //user_id
    Integer copilot; //user_id
    Integer steward1;
    Integer steward2;
    Integer steward3;
    Integer steward4;
    Integer steward5;
    String status="";
    String unexpected_landing; //iata

    public Flight() {
    }

    public Integer getDep_gate() {
        return dep_gate;
    }

    public void setDep_gate(Integer dep_gate) {
        this.dep_gate = dep_gate;
    }

    public Integer getArr_gate() {
        return arr_gate;
    }

    public void setArr_gate(Integer arr_gate) {
        this.arr_gate = arr_gate;
    }

    public Integer getPlane_id() {
        return plane_id;
    }

    public void setPlane_id(Integer plane_id) {
        this.plane_id = plane_id;
    }

    public Integer getCompany() {
        return company;
    }

    public void setCompany(Integer company) {
        this.company = company;
    }

    public Integer getPilot() {
        return pilot;
    }

    public void setPilot(Integer pilot) {
        this.pilot = pilot;
    }

    public Integer getCopilot() {
        return copilot;
    }

    public void setCopilot(Integer copilot) {
        this.copilot = copilot;
    }

    public Integer getSteward1() {
        return steward1;
    }

    public void setSteward1(Integer steward1) {
        this.steward1 = steward1;
    }

    public Integer getSteward2() {
        return steward2;
    }

    public void setSteward2(Integer steward2) {
        this.steward2 = steward2;
    }

    public Integer getSteward3() {
        return steward3;
    }

    public void setSteward3(Integer steward3) {
        this.steward3 = steward3;
    }

    public Integer getSteward4() {
        return steward4;
    }

    public void setSteward4(Integer steward4) {
        this.steward4 = steward4;
    }

    
    
    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getDep_airport() {
        return dep_airport;
    }

    public void setDep_airport(String dep_airport) {
        this.dep_airport = dep_airport;
    }

    
    public String getArr_airport() {
        return arr_airport;
    }

    public void setArr_airport(String arr_airport) {
        this.arr_airport = arr_airport;
    }

    
   

    public int getReserved_seats() {
        return reserved_seats;
    }

    public void setReserved_seats(int reserved_seats) {
        this.reserved_seats = reserved_seats;
    }

    public Time getDuration() {
        return duration;
    }

    public void setDuration(Time duration) {
        this.duration = duration;
    }

    public Timestamp getTakeoff_time() {
        return takeoff_time;
    }

    public void setTakeoff_time(Timestamp takeoff_time) {
        this.takeoff_time = takeoff_time;
    }

    public Timestamp getPlanned_arrival() {
        return planned_arrival;
    }

    public void setPlanned_arrival(Timestamp planned_arrival) {
        this.planned_arrival = planned_arrival;
    }

    public Timestamp getExpected_arrival() {
        return expected_arrival;
    }

    public void setExpected_arrival(Timestamp expected_arrival) {
        this.expected_arrival = expected_arrival;
    }

    public Timestamp getArrival_time() {
        return arrival_time;
    }

    public void setArrival_time(Timestamp arrival_time) {
        this.arrival_time = arrival_time;
    }

    
    
    

    public void setSteward4(int steward4) {
        this.steward4 = steward4;
    }

    public Integer getSteward5() {
        return steward5;
    }

    public void setSteward5(Integer steward5) {
        this.steward5 = steward5;
    }

   
    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public String getUnexpected_landing() {
        return unexpected_landing;
    }

    public void setUnexpected_landing(String unexpected_landing) {
        this.unexpected_landing = unexpected_landing;
    }
    
    
}
