using UnrealBuildTool;
using System.IO;

public class JustFrame : ModuleRules {
  public JustFrame(ReadOnlyTargetRules Target) : base(Target) {
    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

    // SDL3 Static Integration
    string SDL3Path = Path.Combine(ModuleDirectory, "../../ThirdParty/SDL3");
    string SDL3IncludePath = Path.Combine(SDL3Path, "include");
    string SDL3LibPath = Path.Combine(SDL3Path, "lib", "libSDL3.a");

    PublicIncludePaths.Add(SDL3IncludePath);
    PublicAdditionalLibraries.Add(SDL3LibPath);

    PublicDefinitions.Add("SDL_STATIC=1");
    PublicDefinitions.Add("SDL_MAIN_HANDLED=1");

    // Core engine dependencies
    PublicDependencyModuleNames.AddRange(new string[] {
      "Core", "CoreUObject", "Engine", "Json", "JsonUtilities", "OpenSSL"
    });

    PrivateDependencyModuleNames.AddRange(new string[] {
      // private modules here
    });

    // Editor-specific modules when building the editor
    if (Target.bBuildEditor) {
      PrivateDependencyModuleNames.AddRange(new string[] {
        "UnrealEd",
        "EditorSubsystem",
        "Slate",
        "SlateCore"
      });
    }
  }
}