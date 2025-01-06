package ticketingsystem;

import java.util.ArrayList;
import java.util.List;

import ticketingsystem.receipts.EmailReceipt;
import ticketingsystem.receipts.PostReceipt;
import ticketingsystem.receipts.Receipt;
import ticketingsystem.receipts.SMSReceipt;

public class TicketingSystem {
    private int ticketsLeft;
    private List<Receipt> receipts;
    private static TicketingSystem instance = null;

    public TicketingSystem(int numTickets) {
        this.ticketsLeft = numTickets;
        this.receipts = new ArrayList<>();
    }

    public static TicketingSystem getInstance(int numTickets) {
        if (instance == null) {
            instance = new TicketingSystem(numTickets);
        }
        return instance;
    }

    public int getTicketsLeft() {
        return ticketsLeft;
    }

    private void setTicketsLeft(int ticketsLeft) {
        this.ticketsLeft = ticketsLeft;
    }

    public String buyTicket(String email, String sms, String address) {
        String msg = "";
        if (getTicketsLeft() > 0) {
            setTicketsLeft(getTicketsLeft() - 1);
            msg += "There is now " + getTicketsLeft() + " left!" + "\n";
            Receipt receipt = new Receipt();
            double cost = receipt.getCost();
            msg += receipt.send();
            if (email != null) {
                Receipt emailReceipt = new EmailReceipt(email);
                cost += emailReceipt.getCost();
                msg += emailReceipt.send();
                receipts.add(emailReceipt);
            }
            if (sms != null) {
                Receipt smsReceipt = new SMSReceipt(sms);
                cost += smsReceipt.getCost();
                msg += smsReceipt.send();
                receipts.add(smsReceipt);
            }
            if (address != null) {
                Receipt postReceipt = new PostReceipt(address);
                cost += postReceipt.getCost();
                msg += postReceipt.send();
                receipts.add(postReceipt);
            }

            msg += "The total cost is: " + cost + "\n";
            msg += "------------------------------------------";
        } else {
            msg += "Unfortunately there are no tickets left :(";
        }
        System.out.println(msg);
        return msg;
    }

}
