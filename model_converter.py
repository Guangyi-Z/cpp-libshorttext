# -*- coding: utf-8 -*-

'''model_converter.py model_path
'''

import cPickle
import sys
import os
import shutil


def pk_load(path):
    return cPickle.load(open(path, 'rb'))


if __name__ == '__main__':
    if len(sys.argv) != 2:
        sys.stderr.write(__doc__)
        sys.exit(1)
    model_path = sys.argv[1]

    model_dirname = os.path.basename(model_path)
    model_dirname2 = model_dirname + '_converted'
    model_path2 = os.path.join(
        os.path.dirname(model_path),
        model_dirname2)
    if os.path.exists(model_path2):
        shutil.rmtree(model_path2)
    os.makedirs(model_path2)

    options = pk_load(os.path.join(model_path, 'learner/options.pickle'))
    print(options)
    with open(os.path.join(model_path2, 'options.txt'), 'w') as fout:
        for op in options:
            fout.write(op.strip() + '\n')

    # no idf
    # idf = pk_load(os.path.join(model_path, 'learner/idf.pickle'))

    idx2tok = pk_load(os.path.join(model_path, 'converter/text_prep.config.pickle'))
    print(idx2tok['option'])
    with open(os.path.join(model_path2, 'text_prep.txt'), 'w') as fout:
        for tok in idx2tok['idx2tok']:
            fout.write(tok + '\n')

    fidx2ngram = pk_load(os.path.join(model_path, 'converter/feat_gen.config.pickle'))
    print(fidx2ngram['option'])
    with open(os.path.join(model_path2, 'feat_gen.txt'), 'w') as fout:
        start = True
        for ngram in fidx2ngram['fidx2ngram']:
            if start:
                fout.write('{0}\n'.format(ngram))
                start = False
            else:
                fout.write('{0}\n'.format('\t'.join(map(lambda x: str(x), ngram))))

    idx2class = pk_load(os.path.join(model_path, 'converter/class_map.config.pickle'))
    print(idx2class['option'])
    with open(os.path.join(model_path2, 'class_map.txt'), 'w') as fout:
        for _cls in idx2class['idx2class']:
            fout.write(_cls + '\n')
