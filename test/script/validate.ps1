$ErrorActionPreference = "Stop"

$tool = 'C:\project\xml_code_generator\workspace\codeblocks\bin\Debug\xml_code_generator.exe'

. "$PSScriptRoot\helper.ps1"

function validateTool
{
    param($schema, $option, $path, $source)

    if(!$path)
    {
        $path = $schema
    }

    echo "path: $path, schema: $schema"
    
    generateSource -tool $tool -referencePath "$pwd/reference/$path" -targetPath "$pwd/generated/$path" -name $schema -option $option -source $source
    if($LastExitCode -eq 0)
    {
        echo "Successfully validated for $schema.xsd"
    }
    else
    {
        Write-Error "Failed to validate xml_generator_tool for $schema.xsd"
        exit 1
    }
    echo ""
}

function setup
{
    cd ..
    if(Test-Path 'generated') 
    {
        Remove-Item 'generated' -Recurse -Force
    }
    New-Item -Path 'generated' -ItemType Directory
}

setup
validateTool -schema "food"
validateTool -schema "purchase_order" "-b" -source "PurchaseOrder"

validateTool -schema "shiporder1" "-b" -path "shiporder1" -source "shiporder"
validateTool -schema "shiporder2" "-b" -path "shiporder1" -source "shiporder"
validateTool -schema "shiporder3" "-b" -path "shiporder1" -source "shiporder"

validateTool -schema "shiporder1" @("-b", "-d") -path "shiporder2" -source "shiporder"
validateTool -schema "shiporder2" @("-b", "-d") -path "shiporder2" -source "shiporder"
validateTool -schema "shiporder3" @("-b", "-d") -path "shiporder2" -source "shiporder"

validateTool -schema "shiporder1" @("-d", "-c") -path "shiporder3" -source "shiporder"
validateTool -schema "shiporder2" @("-d", "-c") -path "shiporder3" -source "shiporder"
validateTool -schema "shiporder3" @("-d", "-c") -path "shiporder3" -source "shiporder"