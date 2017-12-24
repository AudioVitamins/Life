<?php
// (c)2016 Andrew Shakinovsky
// submit using POST or GET to this script:
// EMAIL - customer email
// REF - reference string to be stored in the database (optional)
// PROD - product code (1-255) should refer to a trial code (will be validated)
// VER - version (1-255)
// OS - which os product is running on (optional will replace REF)
// DAW - which DAW is being used (optional will replace REF)
// will create/update an email row in trial table and send an email with 
// the unlock code
// outputs:
// SUCCESS
// ERROR xxxxxxx
// where xxxxxxx is text to be shown (eg you've asked for too many trials)
/* table spec:
CREATE TABLE IF NOT EXISTS `trial` (
  `date_issued` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `prod` varchar(3) NOT NULL,
  `ver` varchar(3) NOT NULL,
  `ref` varchar(255) DEFAULT NULL,
  `email` varchar(255) NOT NULL,
  `ipaddress` varchar(32) DEFAULT NULL,
  `ucode` varchar(32) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
*/
   require 'mail/PHPMailerAutoload.php';
   require("db_inc.php");

   // maximum number of times they can request trial
   $max_trials = 2;
   
   // will be valid 14 days
   $days_valid=14;
   if(isset($_GET['EMAIL']) and isset($_GET['PROD']) and isset($_GET['VER'])) {
      // coming in via get parameters (from the vst on the client)
      $email=$_GET['EMAIL'];
      $prod=$_GET['PROD'];
      $ver=$_GET['VER'];
      $ref=$_GET['OS'];
      $ref = $ref . "/";
      $ref = $ref . $_GET['DAW'];      
      $interactive=false;
   } else {      
      // via manual form
      if(!isset($_POST['EMAIL']) or !isset($_POST['PROD']) or !isset($_POST['VER'])) {
         echo "ERROR Insufficient info";
         goto end;
      }
      $ref=$_POST['OS'];
      $ref = $ref . "/";
      $ref = $ref . $_POST['DAW'];      
      $email=$_POST['EMAIL'];
      $prod=$_POST['PROD'];
      $ver=$_POST['VER'];
      $interactive=true;
   }
   
   $ipaddr=$_SERVER['REMOTE_ADDR'];   

   

   // Validate that it's a valid trial code  (add additional ones here)
   
   if($prod=='99') {
      $prod_name="Stochas";
   } else if($prod=='91') {
      $prod_name="Life";
   } else {
      echo "ERROR invalid product code for trial";
      goto end;
   }

   $subject="Trial unlock code for " . $prod_name ;
   $prefix = "Here is your unlock code for the trial version of " . $prod_name . "." ;

   
   if($interactive) {
      //
      // HTML **************************************************
      //
	   echo("<html><body><h1>Trial for $prod_name</h1>\n");	   
      //
      // END HTML **************************************************
      //
   }    
   
   
   
   // make sure they have not requested too many times
   $statement = $pdo->prepare(
      "SELECT count(*) from trial where email = :email and prod = :prod");
   $statement->bindparam(':email', $email, PDO::PARAM_STR,255);
   $statement->bindparam(':prod', $prod, PDO::PARAM_STR,3);
   $statement->bindcolumn(1, $email_count, PDO::PARAM_INT);
   if(!$statement->execute()) {
      echo "ERROR DB Error 1";
      goto end;
   }
   // how many
   if(($row = $statement->fetch(PDO::FETCH_BOUND))==0) {
      echo "ERROR DB Error 2";
      goto end;
   }
   if($email_count >= $max_trials) {
      if($interactive) {
         //
         // HTML **************************************************
         //
         echo("You have reached your allotted number of times that you can try the product.");	   
         //
         // END HTML **************************************************
         //
      } else {
         echo "ERROR Too many trials";
      }
      goto end;
   }
   
   // generate a trial user id   
   $n= mt_rand(10000, 20000); // random number for user id
   $command = realpath(dirname(__FILE__)) . "/bin/codegen USERID " . $n;
   $output = exec($command, $outa, $rv);
   if($rv != 0) {
      echo "ERROR uidgen failed";
      goto end;
   }
   $uid=$output;
   
   // generate the unlock code
   $command = realpath(dirname(__FILE__)) . "/bin/codegen gen " . $uid  . " " . $prod . " " . $ver . " " . $days_valid;
   $output = exec($command, $outa, $rv);
   if($rv != 0) {
      echo "ERROR Codegen failed";
      goto end;
   }
   $trialcode=$output;
   
   // create a row in trial
   $statement = $pdo->prepare("INSERT INTO trial (prod, ver, ref, email, ipaddress, ucode) VALUES (:prod, :ver, :ref, :email, :ipaddr, :ucode)");
   $statement->bindparam(':prod', $prod, PDO::PARAM_STR,3);
   $statement->bindparam(':ver', $ver, PDO::PARAM_STR,3);
   $statement->bindparam(':ref', $ref, PDO::PARAM_STR,255);
   $statement->bindparam(':email', $email, PDO::PARAM_STR,255);
   $statement->bindparam(':ipaddr', $ipaddr, PDO::PARAM_STR,32);
   $statement->bindparam(':ucode', $trialcode, PDO::PARAM_STR,32);
   if(!$statement->execute()) {
      echo "ERROR DB Error 1";
      //print_r($statement->errorInfo());
      goto end;
   }
   
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
   $mail->Body    = $prefix .
           "\nTrial unlock code: " . $trialcode ."\n";
   $mail->AltBody = $prefix .
           "\nTrial unlock code: " . $trialcode ."\n";           
   $mail->addAddress($email);  

   if(!$mail->Send()) {
      echo "ERROR " . $mail->ErrorInfo;
   } else {
      if($interactive) {
         //
         // HTML **************************************************
         //
         echo("An email message has been sent. Please check your email (including your spam folder)");	   
         //
         // END HTML **************************************************
         //
      } else {
         echo "SUCCESS";
      }
   }
   
 end:
 
 if($interactive) {
   //
   // HTML **************************************************
   //
   echo("</html>\n");	   
   //
   // END HTML **************************************************
   //
}    
?>
