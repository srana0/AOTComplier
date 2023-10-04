public class PrintThree{
    static int return_three(int value){
        value = value + 1;
        value = value + 1;
        value = value + 1;
        return value;
    }
    public static void main (String[] args){
        int value = 0;
        value = return_three(value);
        System.out.println(value);
    }
}