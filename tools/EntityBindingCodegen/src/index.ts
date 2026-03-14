import { loadPinDefinitions } from "./pindefs";
import { parse } from "./parsers";
import { generate } from "./codegen";

window.onload = async () => {
  const page = {
    inputs: {
      binding_name: document.getElementById(
        "binding_name",
      )!! as HTMLInputElement,
      hash_or_path: document.getElementById(
        "hash_or_path",
      )!! as HTMLInputElement,
      hash_or_path_suggestions: document.getElementById(
        "hash_or_path_suggestions",
      )!!,
      entity_json: document.getElementById(
        "entity_json",
      )!! as HTMLTextAreaElement,
      entity_json_file: document.getElementById(
        "entity_json_file",
      )!! as HTMLInputElement,
      generate_button: document.getElementById(
        "generate",
      )!! as HTMLButtonElement,
      clear_button: document.getElementById("clear")!! as HTMLButtonElement,
    },
    outputs: {
      copy_button: document.getElementById(
        "copy_button",
      )!! as HTMLButtonElement,
      output_area: document.getElementById(
        "output_area",
      )!! as HTMLTextAreaElement,
    },
  };

  function validatePageElements(container: Record<string, HTMLElement | null>) {
    for (const key in container) {
      if (container[key] === null) {
        throw new Error(`Element '${key}' not found`);
      }
    }
  }
  validatePageElements(page.inputs);
  validatePageElements(page.outputs);

  const pinDefHelper = await loadPinDefinitions();

  page.inputs.hash_or_path_suggestions.innerHTML = "";
  pinDefHelper.pinDefinitions.forEach((def) => {
    const option = document.createElement("option");
    option.value = def.path;
    page.inputs.hash_or_path_suggestions.appendChild(option);
  });

  page.inputs.generate_button.onclick = () => {
    const name = page.inputs.binding_name.value.trim();
    let hashOrPath = page.inputs.hash_or_path.value.trim();
    const entityJson = page.inputs.entity_json.value.trim();

    if (entityJson === "") {
      alert("Please fill in all input fields.");
      return;
    }

    const entityInfo = parse(JSON.parse(entityJson));
    if (!entityInfo) {
      alert("Failed to parse entity JSON. Please check the format.");
      return;
    }

    if (name && name !== "") {
      entityInfo.metadata.name = name;
    }

    console.log(entityInfo);

    if (
      hashOrPath == "" &&
      (entityInfo.metadata.path || entityInfo.metadata.hash)
    ) {
      hashOrPath = entityInfo.metadata.path || entityInfo.metadata.hash || "";
      page.inputs.hash_or_path.value = hashOrPath;
    }

    if (hashOrPath && hashOrPath !== "") {
      const pinDefs = pinDefHelper.findPinDefinition(hashOrPath);
      console.log(hashOrPath, pinDefs);

      if (pinDefs) {
        console.log("pinDefs:", pinDefs.in, pinDefs.out);

        for (const pin of pinDefs.in) {
          entityInfo.pins.push({
            name: pin.pin,
            direction: "input",
            description: pin.description,
          });
        }

        for (const pin of pinDefs.out) {
          entityInfo.pins.push({
            name: pin.pin,
            direction: "output",
            description: pin.description,
          });
        }

        entityInfo.metadata.path = pinDefs.path;
      } else {
        entityInfo.metadata.path = hashOrPath;
      }
    }

    const code = generate(entityInfo, pinDefHelper);

    page.outputs.output_area.value = code;
    page.outputs.copy_button.disabled = false;
  };

  page.inputs.clear_button.onclick = () => {
    page.inputs.binding_name.value = "";
    page.inputs.hash_or_path.value = "";
    page.inputs.entity_json.value = "";
    page.inputs.entity_json_file.value = "";
    page.outputs.output_area.value = "";
  };

  page.outputs.copy_button.onclick = () => {
    const outputText = page.outputs.output_area.value;
    navigator.clipboard.writeText(outputText);
  };

  page.inputs.entity_json_file.onchange = (event) => {
    // @ts-expect-error - idk why??
    const file = event.target?.files?.[0];
    if (file) {
      const reader = new FileReader();
      reader.onload = (e) => {
        page.inputs.entity_json.value = e.target?.result as string;
      };
      reader.readAsText(file);
    }
  };
};
