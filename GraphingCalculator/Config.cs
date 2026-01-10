using System.IO;
using Newtonsoft.Json;

namespace GraphingCalculator;

public class Config
{
    public string Expression { get; init; }
    public int Thickness { get; init; }
    public float ScaleX { get; set; }
    public float ScaleY { get; set; }
    public float Step { get; init; }
    public float AxesOpacity { get; init; }
    public float AxesThickness { get; init; }

    public static Config Load()
    {
        string json = File.ReadAllText(Path.Combine("..", "..", "..", "config", "expression.json"));
        return JsonConvert.DeserializeObject<Config>(json);
    }
}