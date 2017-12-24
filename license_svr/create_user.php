<?php
// (c)2016 Andrew Shakinovsky
// submit using POST to this script:
// REF - reference string to be stored in the database
// NAME - customer name
// EMAIL - customer email
// PROD - product code (1-255)
// VER - version (1-255)
// EXPIRE - optional expiration date (will not allow codegen after this date) YYYY-MM-DD
// will create a user id, a password, insert a row in customer table and email the customer
// with the user id and password
// returns 
// SUCCESS xxxxx yyyyyy
// Where xxx is the user id and yyyyyy is the password
// If there is a problem, a brief error string is returned as follows:
// ERROR xxxxxxx
// test valid user id 21274584     

   function randomPassword() {
    $alphabet = 'abcdefghjkmnpqrstuvwxyzABCDEFGHJKMNPQRSTUVWXYZ23456789';
    $pass = array(); //remember to declare $pass as an array
    $alphaLength = strlen($alphabet) - 1; //put the length -1 in cache
    for ($i = 0; $i < 10; $i++) {
        $n = mt_rand(0, $alphaLength);
        $pass[] = $alphabet[$n];
    }
    return implode($pass); //turn the array into a string
   }

   require("../db_inc.php");
   
   if(!isset($_POST['REF']) or !isset($_POST['NAME']) or !isset($_POST['EMAIL']) or !isset($_POST['PROD']) or !isset($_POST['VER'])) {
      echo "ERROR Insufficient info";
      goto end;
   }
     
   $fname=$_POST['FNAME'];  
   $ref=$_POST['REF'];
   $name=$_POST['NAME'];
   $email=$_POST['EMAIL'];
   $prod=$_POST['PROD'];
   $ver=$_POST['VER'];
   if(isset($_POST['EXPIRE'])) {
      $dt=DateTime::createFromFormat('Y-m-d', $_POST['EXPIRE']);
      $dtnow=DateTime::createFromFormat('Y-m-d', date('Y-m-d'));
      if($dt and $dt > $dtnow)
         $expires=$_POST['EXPIRE'];      
   }
   


   if($prod=='100') {
      $prod_name="Stochas";
   } else if($prod=='116') {
      $prod_name="Life";
   } else {
      echo "ERROR invalid product code";
      goto end;
   }
   
  

   if(isset($_POST['SUBJECT'])) {
      $subject=$_POST['SUBJECT'];
   }else {
      $subject="Unlock code";
   }

   // create a row in customer
   $statement = $pdo->prepare("INSERT INTO customer (prod, ver, ref, email, expires, name) VALUES (:prod, :ver, :ref, :email, :expires, :name)");
   $statement->bindparam(':ref', $ref, PDO::PARAM_STR,255);
   $statement->bindparam(':email', $email, PDO::PARAM_STR,255);
   $statement->bindparam(':name', $name, PDO::PARAM_STR,255);
   $statement->bindparam(':prod', $prod, PDO::PARAM_STR,3);
   $statement->bindparam(':ver', $ver, PDO::PARAM_STR,3);
   $statement->bindparam(':expires', $expires, PDO::PARAM_STR,32);
   if(!$statement->execute()) {
      echo "ERROR DB Error 1";
      //print_r($statement->errorInfo());
      goto end;
   }
   // generate a user id   
   $dbid=$pdo->lastInsertId();
   $command = realpath(dirname(__FILE__)) . "/../bin/codegen USERID " . $dbid;
   $output = exec($command, $outa, $rv);
   if($rv != 0) {
      echo "ERROR uidgen failed: $command";
      goto end;
   }
   $uid=$output;
   // generate a password
   $pwd=randomPassword();
   
   // update table with new info   
   $statement = $pdo->prepare(
      "UPDATE customer SET uid=:uid, pwd=:pwd WHERE id=:dbid");      
   $statement->bindparam(':uid', $uid, PDO::PARAM_STR,16);
   $statement->bindparam(':pwd', $pwd, PDO::PARAM_STR,16);   
   $statement->bindparam(':dbid', $dbid, PDO::PARAM_INT);   
   if(!$statement->execute()) {
      echo "ERROR DB Error 2";
      goto end;
   }
   
require '../mail/PHPMailerAutoload.php';

   // send message

$mail = new PHPMailer(); // create a new object
$mail->IsSMTP(); // enable SMTP
$mail->SMTPDebug = 0; // debugging: 1 = errors and messages, 2 = messages only
$mail->SMTPAuth = true; // authentication enabled
$mail->SMTPSecure = 'tls'; // secure transfer enabled REQUIRED for Gmail
$mail->Host = 'auth.smtp.1and1.co.uk';
$mail->Port = 587; // or 587
$mail->IsHTML(true);
$mail->Username = $admin_email;
$mail->Password = $mail_password;
$mail->SetFrom($from_email, "Audio Vitamins");
$mail->Subject = "Product Authorisation for " . $prod_name;

if($prod=='100') {

$mail->Body    = "Hi " . $fname . "<br/><br/>Here are the details needed to unlock your product.<br /><br />Please click on the registration button in the settings tab and enter the Purcahse ID and Purchase Code.<br /><br />Purchase ID: " . $uid . "<br />Purchase Code: " . $pwd . "<br /><br />The downloads:<br /><a href=\"https://www.audiovitamins.com/download/6611/\">Stochas Windows</a><br /><a href=\"https://www.audiovitamins.com/download/6609/\">Stochas OSX</a><br /><a href=\"http://installers.audiovitamins.com/user-guides/Stochas-UserGuide.pdf\">Stochas UserGuide</a><br /><br />Please save this email in case you need to enter these details again.<br /><br />Also, please do not share this information with anyone, as your account may be locked out if there is abnormal activity.";
$mail->AltBody = "Hi " . $fname . "\n\nHere are the details needed to unlock your product.\n\nPlease click on the registration button in the settings tab and enter the user id and password.\n\nPurchase ID: " . $uid . "\nPurchase Code: " . $pwd . "\n\nPlease save this email in case you need to enter these details again. \n\nPlease do not share this information with anyone, as your account may be locked out if there is abnormal activity.";

} else if($prod=='116') {

$mail->Body    = "Hi " . $fname . "<br/><br/>Here are the details needed to unlock your product.<br /><br />Please click on the registration button in the settings tab and enter the Purcahse ID and Purchase Code.<br /><br />Purchase ID: " . $uid . "<br />Purchase Code: " . $pwd . "<br /><br />The downloads:<br /><a href=\"FIX_ME\">Life Windows</a><br /><a href=\"FIX_ME\">Life OSX</a><br /><a href=\"FIX_ME.pdf\">Life UserGuide</a><br /><br />Please save this email in case you need to enter these details again.<br /><br />Also, please do not share this information with anyone, as your account may be locked out if there is abnormal activity.";
$mail->AltBody = "Hi " . $fname . "\n\nHere are the details needed to unlock your product.\n\nPlease click on the registration button in the settings tab and enter the user id and password.\n\nPurchase ID: " . $uid . "\nPurchase Code: " . $pwd . "\n\nPlease save this email in case you need to enter these details again. \n\nPlease do not share this information with anyone, as your account may be locked out if there is abnormal activity.";

}

$mail->addAddress($email);  

 if(!$mail->Send()) {
    echo "ERROR " . $mail->ErrorInfo;
 } else {
    echo "SUCCESS " . $uid . " " . $pwd;
 }

   // output the info
   
   

 end:
?>
