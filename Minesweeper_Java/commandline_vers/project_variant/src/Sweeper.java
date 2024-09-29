public class Sweeper {
    private int side;
    private int mines;

    public Sweeper(int side, int mines) {
        this.side = side;
        this.mines = mines;
    }

    public int getSide() {
        return side;
    }

    public void setSide(int side) {
        this.side = side;
    }

    public int getMines() {
        return mines;
    }

    public void setMines(int mines) {
        this.mines = mines;
    }
}
