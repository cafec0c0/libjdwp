public class SimpleServer {


    public static void main(String[] args) throws InterruptedException {
        AnotherClass thing = new AnotherClass();

        System.out.println("Java application running");
        while (true) {
            thing.square(2); // Prevent class from being unloaded
            Thread.sleep(100);
        }
    }

}