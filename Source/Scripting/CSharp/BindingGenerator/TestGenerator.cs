using System;
using System.Collections.Generic;
using System.Text;
using CppSharp;
using CppSharp.AST;
using CppSharp.Generators;
using CppSharp.Passes;

namespace TriVEngineCSharp.Source.Scripting.CSharp.BindingGenerator
{
    public class TestGenerator : ILibrary
    {
        public void Preprocess(Driver driver, ASTContext ctx)
        {

        }

        public void Postprocess(Driver driver, ASTContext ctx)
        {

        }

        public void Setup(Driver driver)
        {
            var options = driver.Options;
            options.GeneratorKind = GeneratorKind.CSharp;
            var module = options.AddModule("TriVEngine");
            module.IncludeDirs.Add(@"A:\Projekte\Visual Studio Workspace\2017\TriVEngine\Source\Engine\Core");
            module.Headers.Add("EngineClock.hpp");
            // module.LibraryDirs.Add();
            options.OutputDir = @"A:\Projekte\Visual Studio Workspace\2017\TriVEngine\Source\Scripting\CSharp\Generated";
        }

        public void SetupPasses(Driver driver)
        {
            driver.Context.TranslationUnitPasses.RenameDeclsUpperCase(RenameTargets.Any);
            driver.Context.TranslationUnitPasses.AddPass(new FunctionToInstanceMethodPass());

        }

        public static void Main(string[] args)
        {
            ConsoleDriver.Run(new TestGenerator());
            Console.WriteLine("Finished!");
            Console.ReadLine();
        }
    }
}
