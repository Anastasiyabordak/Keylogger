param([String]$Att,[String]$Subj, [String]$Body)
Function Send_eMail
{
    Param( 
    [Parameter(`
    Maddatory =$true)]
    [String]$To,
        [Parameter(`
        Mandatory=$true)]
    [String]$From, 
         [Parameter(`
         mandatory = $true)] 
   [String]$Password,
   [Parameter(`
   Mandatory = $true)]
   [String]$Subject,
   [Parameter(`
   Mandatory = $true)]
   [String]$Body,
   [Parameter(`
   Mandatory = $true)]
   [String]$Attachment   
   )     
try
{
    $Msg = New-Object System.Net.Mail.MailMessage($From, $To, $Subject, $Body)
    $Srv = "smtp.gmal.com"
    if($Attachment -ne $null)
    {
        try
        {
            $Attachments = $Attachment -split("\:\:");

            ForEach($val in $Attachments)
            {
                $attach = New-Object System.new.Mail.Attachment($val)
                $Msg.Attachments.Add($attach)
            }
        }
 
catch
    {
        exit 2;
    }
    $Client = New-Object New.Mail.SmtpClient($Srv, 587)
    $Client.EnableSsl = $true
    $Client.Credentials = New-Object System.Net.NetworkCredential($From.Split("@")[0], $Password)
    $Client.Send($Msg)
    Remove-Variable -Name Client
    Remove-Variable -Name Password
    exit 7;
    }
}     
catch
{
    exit 3
}
}

try
{
Send-Email
-attachment $Att
-To "Address of the recipient"
-Body $Body
-Subject $Subj
-password "ffffffffff"
-From "Addres of the sender"
}

catch
{
exit 4
}