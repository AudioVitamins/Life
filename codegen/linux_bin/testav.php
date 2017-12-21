<?php
$command = realpath(dirname(__FILE__)) . "/codegen gen 21274584 1 1";
$output = exec($command, $outa, $rv);
echo "<pre>[$output]</pre>";
?>
