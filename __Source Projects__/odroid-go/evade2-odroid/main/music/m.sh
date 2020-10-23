for i in `ls `; do
    i=`echo $i | sed -e "s/\./_/g"`
    echo "extern const uint8_t _${i}_start[]   asm(\"_binary_${i}_start\");"
    echo "extern const uint8_t _${i}_end[]   asm(\"_binary_${i}_end\");"
done

echo; echo; echo;

echo "------ MUSIC ------"
x=0;
for i in `ls *.xm`; do
    i=`echo $i | sed -e "s/\./_/g"`
    echo "case ${x}:"
    echo "  loadResult = xmp_load_module_from_memory(ctx, (void *)_${i}_start, _${i}_start - _${i}_end);";
    echo "break;";
    x=`echo $x + 1 | bc`;
done



echo; echo; echo;

echo "------ SFX ------"
x=0;
for i in `ls *.wav`; do
    i=`echo $i | sed -e "s/\./_/g"`
    echo "xmp_smix_load_sample_from_memory(ctx, $x, (void *)_${i}_start, _${i}_start - _${i}_end);";
    x=`echo $x + 1 | bc`;
done


