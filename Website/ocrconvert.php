<?php
$out = '';
exec( 'ocrconvert', $out );
echo $out;
?>