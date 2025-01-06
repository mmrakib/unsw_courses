package ticketingsystem.receipts;

public class EmailReceipt extends Receipt {
    private String email;
    private static final double FEE = 0;

    public EmailReceipt(String email) {
        this.email = email;
    }

    public String send() {
        String msg = "Sending an email to " + email + "\n";
        return msg;
    }

    public double getCost() {
        return FEE;
    }
}
