/**
 * Created by zouxuan on 4/26/17.
 */
go=0;
(function (global) {
    'use strict'

    function createBlock(name, value, content) {
        var item = elem('div', {'class': 'block', draggable: true, 'data-name': name}, [name]);
        if (value !== undefined && value !== null) {
            item.appendChild(elem('input', {type: 'number', value: value,'class': 'text_input'}));
            if (Array.isArray(content)) {
                item.appendChild(elem('div', {'class': 'container2'}, content.map(function (block) {
                    return createBlock.apply(null, block);
                })));
            }
        } else if (typeof content === 'string') {
            item.appendChild(document.createTextNode(' ' + content));
        }
        return item;
    }

    function blockContents(block) {
        var container = block.querySelector('.container2');
        return container ? [].slice.call(container.children) : null;
    }

    function blockValue(block) {
        var input = block.querySelector('input');
        return input ? Number(input.value) : null;
    }

    function blockUnits(block) {
        if (block.children.length > 1 && block.lastChild.nodeType === Node.TEXT_NODE && block.lastChild.textContent) {
            return block.lastChild.textContent.slice(1);
        }
    }

    function blockScript(block) {
        var script = [block.dataset.name];
        var value = blockValue(block);
        if (value !== null) {
            script.push(blockValue(block));
        }
        var contents = blockContents(block);
        var unit = blockUnits(block);
        if (contents) {
            script.push(contents.map(blockScript));
        }
        if (unit) {
            script.push(unit);
        }
        return script.filter(function (notNull) {
            return notNull !== null;
        });
    }

    function runBlocks(block) {
		
        block.forEach(function (block) {
            trigger('run', block);
        });
       
	
    }
	
    global.Block = {
        create: createBlock,
        value: blockValue,
        contents: blockContents,
        script: blockScript,
        run: runBlocks,
        trigger: trigger
    };

    
   
     
})(window);



