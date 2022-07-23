function compareFile 
{
    param ($source, $target)

    # Check if reference source file exists
    if(-not(Test-Path -Path $source -PathType Leaf)) 
    {
        echo "Reference source file does not exist: $referenceSource"    
        exit 2
    }

    # Check if target source file exists
    if(-not(Test-Path -Path $target -PathType Leaf)) 
    {
        echo "target source file does not exist: $targetSource"    
        exit 3
    }

    $old = Get-Content $source -raw
    $new = Get-Content $target -raw

    if(-not($old -ceq $new))
    {
        echo "source file doesn't match with target file"
        echo $source
        exit 4
    }
}

function generateSource
{
    param ($tool, $referencePath, $targetPath, $name, $options, $source)
    
    $schema = "$referencePath\$schema.xsd"

    echo "Parsing XML schema: $schema"

    # Remove target path if it already exists
    if(Test-Path -Path $targetPath) 
    {
        Remove-Item $targetPath -Recurse -Force
    }

    New-Item -Path $targetPath -ItemType Directory        
    cd $targetPath

    # Execute xml_code_generator to parse reference schema file and generate source code
    echo "$tool $options $schema"

    & $tool $options $schema 
    if($LastExitCode -ne 0)
    {
        echo "Failed to genrate source code. "
        exit 1
    }

    if(!$source)
    {
        $source = $name
    }

    # Compare generated source
    $referenceSource = "$referencePath\$source.c"
    $targetSource = "$targetPath\$source.c"
    compareFile -source $referenceSource -target $targetSource

    # Compare generated header
    $referenceHeader = "$referencePath\$source.h"
    $targetHeader = "$targetPath\$source.h"
    compareFile -source $referenceHeader -target $targetHeader
    cd ../..
}
