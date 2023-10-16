public class Main
{
    public static void main(String[] args)
    {
        Matrix matrix1 = new Matrix(5, 4);
        matrix1.traverse();

        Matrix matrix2 = new Matrix(Constants.BACK_UP_MATRIX);
        matrix2.traverse();
    }
}
