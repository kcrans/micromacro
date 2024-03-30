import Config 
import Figlet
import ArgumentParser

@main
struct FigletTool : ParsableCommand {
    @Option(help: "Specify the device name for the serial port")
    public var input: String

    public func run() throws{
        Figlet.say(self.input)
        print(int(MyMacro))
    }
}
